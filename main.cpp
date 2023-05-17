#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "Shader.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string loadShaderSrc(const char *filename);

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);
Joystick mainJ(0);

int main()
{
    std::cout << "Hello world\n";

    int success;
    char infoLog[512];

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef GL_TEXTURE_APPLICATION_MODE_EXT
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "Pluto", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Couldn't be created\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, Keyboard::keyCallback);

    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);

    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);

    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    // Shaders
    Shader shader("./assets/vertex_core.glsl", "./assets/fragment_core.glsl");

    float vertices[] = {
        // positions		// colors			// texture coordinates
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.75f, 0.0f, 1.0f, // top left
        0.5f, -0.5f, 0.0f, 0.6f, 1.0f, 0.2f, 1.0f, 0.0f,  // bottom right
        0.5f, 0.5f, 0.0f, 1.0f, 0.2f, 1.0f, 1.0f, 1.0f    // top right
    };
    unsigned int indices[] = {
        0, 1, 2, // first triangle
        3, 1, 2  // second triangle
    };

    // VAO/VBO
    unsigned int VAO,
        VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attributes

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // load image
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/obama.jpg", &width, &height, &nChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("./assets/trump.jpg", &width, &height, &nChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }

    stbi_image_free(data);

    shader.activate();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    shader.activate();
    glm::mat4 trans = glm::mat4(1.0);

    mainJ.update();
    if (mainJ.isPresent())
    {
        std::cout << mainJ.getName() << " is present.\n";
    }
    else
    {
        std::cout << "Joystick not present.\n";
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // set background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // draw shapes
        glBindVertexArray(VAO);
        shader.activate();

        shader.setFloat("mixVal", mixVal);
        shader.setMat4("transform", transform);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (Keyboard::key(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (Keyboard::key(GLFW_KEY_UP))
    {
        mixVal += 0.005f;
        if (mixVal >= 1.0f)
            mixVal = 1.0f;
    }
    if (Keyboard::key(GLFW_KEY_DOWN))
    {
        mixVal -= 0.005f;
        if (mixVal <= 0.0f)
            mixVal = 0.0f;
    }

    // if (Keyboard::key(GLFW_KEY_W))
    // {
    //     transform = glm::translate(transform, glm::vec3(0.0f, 0.1f, 0.0f));
    // }
    // if (Keyboard::key(GLFW_KEY_S))
    // {
    //     transform = glm::translate(transform, glm::vec3(0.0f, -0.1f, 0.0f));
    // }
    // if (Keyboard::key(GLFW_KEY_A))
    // {
    //     transform = glm::translate(transform, glm::vec3(-0.1f, 0.0f, 0.0f));
    // }
    // if (Keyboard::key(GLFW_KEY_D))
    // {
    //     transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
    // }

    mainJ.update();

    float lx = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
    float ly = -mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);

    if (std::abs(lx) > 0.5f)
    {
        transform = glm::translate(transform, glm::vec3(lx / 10, 0.0f, 0.0f));
    }
    if (std::abs(ly) > 0.5f)
    {
        transform = glm::translate(transform, glm::vec3(0.0f, ly / 10, 0.0f));
    }
}

std::string loadShaderSrc(const char *filename)
{
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";

    file.open(filename);

    if (file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        std::cout << "Could not open " << filename << std::endl;
    }
    file.close();

    return ret;
}
