#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <time.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <core/shader.h>
#include <core/camera.h>
#include <core/core.h>
#include <core/model.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window);
void renderCube();
void renderQuad(Shader shader);

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
bool inverseNormals = true;

unsigned int createCubeVBO()
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // left face
        -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        // right face
        1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f  // top-left
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return VBO;
}

unsigned int createQuadVBO()
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

    float vertices[6 * 8];
    for (uint i = 0; i < 6; i++)
    {
        vertices[i * 8] = positions[i].x;
        vertices[i * 8 + 1] = positions[i].y;
        vertices[i * 8 + 2] = positions[i].z;
        vertices[i * 8 + 3] = normal.x;
        vertices[i * 8 + 4] = normal.y;
        vertices[i * 8 + 5] = normal.z;
        vertices[i * 8 + 6] = uvs[i].x;
        vertices[i * 8 + 7] = uvs[i].y;
    }

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return VBO;
}

unsigned int
createVAO(unsigned int VBO)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VAO;
}

struct PointLight
{
    glm::vec3 position;
    glm::vec3 color;
};

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

    srand(time(NULL));

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    unsigned int quadVAO = createVAO(createQuadVBO());
    unsigned int cubeVAO = createVAO(createCubeVBO());

    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int gColorSpec;
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

    unsigned int attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader gBufferShader("g_buffer.vs", "g_buffer.fs");
    Shader lightShader("ssao.vs", "light.fs");
    Shader ssaoShader("ssao.vs", "ssao.fs");
    Shader ssaoBlurShader("ssao.vs", "ssao_blur.fs");
    Shader lightCubeShader("lightCube.vs", "lightCube.fs");
    Shader screenShader("screen_shader.vs", "screen_shader.fs");
    stbi_set_flip_vertically_on_load(1);
    Model backpack("../../../resources/backpack/backpack.obj");

    gBufferShader.use();
    glm::mat4 model(1.0);
    gBufferShader.setMatrix4("model", model);
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
    gBufferShader.setMatrix4("projection", projection);

    PointLight light{
        glm::vec3(2.0f, 4.0f, -2.0f),
        glm::vec3(0.2f, 0.2f, 0.7f),
    };

    lightShader.use();
    lightShader.setVec3("light.color", light.color);
    lightShader.setFloat("light.linear", 0.09f);
    lightShader.setFloat("light.quadratic", 0.032f);
    lightShader.setInt("gPosition", 0);
    lightShader.setInt("gNormal", 1);
    lightShader.setInt("gColorSpec", 2);
    lightShader.setInt("ssaoMap", 3);

    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    const unsigned int KERNEL_SIZE = 64;
    std::vector<glm::vec3> ssaoKernel(KERNEL_SIZE);
    for (unsigned int i = 0; i < KERNEL_SIZE; i++)
    {
        float x = randomFloats(generator) * 2.0f - 1.0f;
        float y = randomFloats(generator) * 2.0f - 1.0f;
        float z = randomFloats(generator);

        glm::vec3 sample(x, y, z);
        sample = glm::normalize(sample);
        float scale = (float)i / (float)KERNEL_SIZE;
        scale = 0.1 + (1.0f - 0.1) * scale * scale; // lerp(0.1, 1.0, scale * scale);
        sample *= scale;
        ssaoKernel[i] = sample;
    }

    const unsigned int NOISE_SIZE = 16;
    std::vector<glm::vec3> ssaoNoise(NOISE_SIZE);
    for (unsigned int i = 0; i < NOISE_SIZE; i++)
    {
        float x = randomFloats(generator) * 2.0f - 1.0f;
        float y = randomFloats(generator) * 2.0f - 1.0f;
        ssaoNoise[i] = glm::vec3(x, y, 0.0f);
    }

    unsigned int noiseTexture;
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, ssaoNoise.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int ssaoFBO;
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    unsigned int ssaoColorBuffer;
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIN_WIDTH, WIN_HEIGHT, 0, GL_RED, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

    unsigned int ssaoBlurFBO;
    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

    unsigned int ssaoBlurTexture;
    glGenTextures(1, &ssaoBlurTexture);
    glBindTexture(GL_TEXTURE_2D, ssaoBlurTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIN_WIDTH, WIN_HEIGHT, 0, GL_RED, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurTexture, 0);

    ssaoShader.use();
    ssaoShader.setInt("gPosition", 0);
    ssaoShader.setInt("gNormal", 1);
    ssaoShader.setInt("ssaoNoise", 2);

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
        // gBuffer pass
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gBufferShader.use();
        gBufferShader.setMatrix4("view", camera.getViewMatrix());

        model = glm::mat4(1.0);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        gBufferShader.setMatrix4("model", model);
        backpack.draw(gBufferShader);

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
        model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
        gBufferShader.setMatrix4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ssao pass
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        ssaoShader.use();
        for (unsigned int i = 0; i < ssaoKernel.size(); i++)
        {
            ssaoShader.setVec3("ssaoKernel[" + std::to_string(i) + "]", ssaoKernel[i]);
        }
        ssaoShader.setMatrix4("projection", projection);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ssao blur pass
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssaoBlurShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // lights pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gColorSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ssaoBlurTexture);
        lightShader.setVec3("viewPos", camera.position);
        glm::vec3 lightViewPos = glm::vec3(camera.getViewMatrix() * glm::vec4(light.position, 1.0f));
        lightShader.setVec3("light.position", lightViewPos);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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