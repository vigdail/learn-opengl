#include <iostream>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <core/shader.h>
#include <core/camera.h>
#include <core/core.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window);
void drawScene(Shader shader);

// settings
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastTime = 0.0f;

core::Camera camera(glm::vec3(0.0f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float mouseLastX = WIN_WIDTH / 2.0f;
float mouseLastY = WIN_HEIGHT / 2.0f;
bool firstMouse = true;

bool KEY_B_PRESSED = false;

unsigned int createWallVBO()
{
    glm::vec3 positions[] = {
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)};

    glm::vec2 uvs[] = {
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f)};

    glm::vec3 normal(0.0f, 0.0f, 1.0f);

    glm::vec3 tangents[6];
    for (unsigned int i = 0; i < 6; i += 3)
    {
        glm::vec3 edge1 = positions[i + 1] - positions[i];
        glm::vec3 edge2 = positions[i + 2] - positions[i];
        glm::vec2 dUV1 = uvs[i + 1] - uvs[i];
        glm::vec2 dUV2 = uvs[i + 2] - uvs[i];

        float f = 1.0 / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

        glm::vec3 tangent = f * (dUV2.y * edge1 - dUV1.y * edge2);

        tangents[i] = tangent;
        tangents[i + 1] = tangent;
        tangents[i + 2] = tangent;
    }

    float vertices[66];
    for (uint i = 0; i < 6; i++)
    {
        vertices[i * 11] = positions[i].x;
        vertices[i * 11 + 1] = positions[i].y;
        vertices[i * 11 + 2] = positions[i].z;
        vertices[i * 11 + 3] = normal.x;
        vertices[i * 11 + 4] = normal.y;
        vertices[i * 11 + 5] = normal.z;
        vertices[i * 11 + 6] = uvs[i].x;
        vertices[i * 11 + 7] = uvs[i].y;
        vertices[i * 11 + 8] = tangents[i].x;
        vertices[i * 11 + 9] = tangents[i].y;
        vertices[i * 11 + 10] = tangents[i].z;
    }

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return VBO;
}

unsigned int
createMeshWithTangent(unsigned int VBO)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VAO;
}

struct State
{
    glm::vec3 lightPosition;
    unsigned int wallVAO;

    unsigned int woodTexture;
    unsigned int woodNormalMap;
    unsigned int woodDispMap;
    unsigned int brickTexture;
    unsigned int brickNormalMap;
    unsigned int brickDispMap;
};

State state;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor == NULL)
    {
        std::cerr << "Failed to get primary monitor" << std::endl;
        glfwTerminate();
        return -1;
    }

    int screenWidth, screeHeight;
    glfwGetMonitorWorkarea(monitor, NULL, NULL, &screenWidth, &screeHeight);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPos(window, (screenWidth - WIN_WIDTH) / 2, (screeHeight - WIN_HEIGHT) / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    state.woodTexture = loadTexture("../../../resources/textures/wood.png");
    state.woodNormalMap = loadTexture("../../../resources/textures/toy_box_normal.png");
    state.woodDispMap = loadTexture("../../../resources/textures/toy_box_disp.png");

    state.brickTexture = loadTexture("../../../resources/textures/bricks2.jpg");
    state.brickNormalMap = loadTexture("../../../resources/textures/bricks2_normal.jpg");
    state.brickDispMap = loadTexture("../../../resources/textures/bricks2_disp.jpg");

    Shader shader("blinn_phong.vs", "blinn_phong.fs");
    unsigned int wallVBO = createWallVBO();
    state.wallVAO = createMeshWithTangent(wallVBO);

    glm::mat4 view;
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

    state.lightPosition = glm::vec3(1.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // -----
        // second pass
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.getViewMatrix();

        shader.use();
        shader.setInt("u_texture", 0);
        shader.setInt("u_normalMap", 1);
        shader.setInt("u_dispMap", 2);
        shader.setVec3("lightPosition", state.lightPosition);
        shader.setVec3("viewPos", camera.position);
        shader.setMatrix4("view", view);
        shader.setMatrix4("proj", projection);
        shader.setFloat("heightScale", 0.1f);

        drawScene(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawScene(Shader shader)
{
    glBindVertexArray(state.wallVAO);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    shader.setMatrix4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, state.woodNormalMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, state.woodDispMap);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(2.5f, 0.0f, -2.0f));
    shader.setMatrix4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.brickTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, state.brickNormalMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, state.brickDispMap);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(core::CameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(core::CameraMovement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(core::CameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(core::CameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !KEY_B_PRESSED)
    {
        KEY_B_PRESSED = true;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        KEY_B_PRESSED = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double x, double y)
{
    if (firstMouse)
    {
        mouseLastX = x;
        mouseLastY = y;
        firstMouse = false;
    }

    float offsetX = x - mouseLastX;
    float offsetY = mouseLastY - y;

    mouseLastX = x;
    mouseLastY = y;

    camera.handleMouseMovement(offsetX, offsetY);
}

void scroll_callback(GLFWwindow *window, double x, double y)
{
    camera.handleMouseScroll(y);
}