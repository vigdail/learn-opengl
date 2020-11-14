#include <iostream>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <core/shader.h>
#include <core/camera.h>
#include <core/core.h>
#include <core/model.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window);

// settings
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastTime = 0.0f;

core::Camera camera(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float mouseLastX = WIN_WIDTH / 2.0f;
float mouseLastY = WIN_HEIGHT / 2.0f;
bool firstMouse = true;

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
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    Shader shader("draw.vs", "draw.fs");

    Model planet("../../../resources/models/planet/planet.obj");
    Model rock("../../../resources/models/rock/rock.obj");

    const unsigned int amount = 1000;
    glm::mat4 *models = new glm::mat4[amount];
    srand(glfwGetTime());
    float radius = 50.0f;
    float offset = 2.5f;

    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);

        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100.0f)) / 100.f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100.0f)) / 100.f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100.0f)) / 100.f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = (rand() % 20) / 100.0f + 0.05f;
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        float rotAngle = rand() % 360;
        model = glm::rotate(model, rotAngle, glm::vec3(0.4, 0.35, 0.7));

        models[i] = model;
    }

    lastTime = glfwGetTime();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 120.0f);

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
        // first pass
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0);

        glm::mat4 view;
        view = camera.getViewMatrix();

        // draw
        shader.use();
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMatrix4("u_model", model);
        shader.setMatrix4("u_view", view);
        shader.setMatrix4("u_proj", projection);
        planet.draw(shader);
        // rocks
        for (unsigned int i = 0; i < amount; i++)
        {
            model = models[i];
            shader.setMatrix4("u_model", model);
            rock.draw(shader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] models;

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