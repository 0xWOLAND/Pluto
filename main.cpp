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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string loadShaderSrc(const char *filename);

int main()
{
    std::cout << "Hello world\n";

    int success;
    char infoLog[512];

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // identity matrix
    // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // translation
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    vec = trans * vec;

    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;

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

    // Shaders

    // compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderSrc = loadShaderSrc("./assets/vertex_core.glsl");
    const GLchar *vertShader = vertShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertShader, NULL);
    glCompileShader(vertexShader);

    // catch error if vertex shader causes an error
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error with vertex shader compilation: " << std::endl
                  << infoLog << std::endl;
    }

    // compile fragment shader
    unsigned int fragmentShaders[2];
    fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderSrc = loadShaderSrc("./assets/fragment_core.glsl");
    const GLchar *fragShader = fragShaderSrc.c_str();
    glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);
    glCompileShader(fragmentShaders[0]);

    // catch error if fragment shader causes an error
    glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
        std::cout << "Error with fragment shader compilation: " << std::endl
                  << infoLog << std::endl;
    }

    // compile second fragment shader
    fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
    fragShaderSrc = loadShaderSrc("./assets/fragment_core2.glsl");
    fragShader = fragShaderSrc.c_str();
    glShaderSource(fragmentShaders[1], 1, &fragShader, NULL);
    glCompileShader(fragmentShaders[1]);

    // catch error if fragment shader causes an error
    glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
        std::cout << "Error with fragment shader compilation: " << std::endl
                  << infoLog << std::endl;
    }

    unsigned int shaderPrograms[2];
    shaderPrograms[0] = glCreateProgram();

    glAttachShader(shaderPrograms[0], vertexShader);
    glAttachShader(shaderPrograms[0], fragmentShaders[0]);
    glLinkProgram(shaderPrograms[0]);

    // catch errors
    glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
        std::cout << "Linking error: " << std::endl
                  << infoLog << std::endl;
    }

    shaderPrograms[1] = glCreateProgram();

    glAttachShader(shaderPrograms[1], vertexShader);
    glAttachShader(shaderPrograms[1], fragmentShaders[1]);
    glLinkProgram(shaderPrograms[1]);

    // catch errors
    glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
        std::cout << "Linking error: " << std::endl
                  << infoLog << std::endl;
    }

    // delete shaders since they have been used now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaders[0]);
    glDeleteShader(fragmentShaders[1]);

    // Vertex Array
    // float vertices[] = {
    //     -0.5, -0.5, 0.0,
    //     0.5, 0.5, 0.0,
    //     0.5, -0.5, 0.0,
    //     -0.5, 0.5, 0.0};

    // indices
    // unsigned int indices[] = {
    //     0, 1, 2,
    //     0, 3, 1};

    float vertices[] = {
        // first triangle
        -0.5f, -0.5f, 0.0f,
        -0.25f, 0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,

        // second triangle
        0.5f, -0.5f, 0.0f,
        0.25f, 0.5f, 0.0f,
        0.1f, -0.5f, 0.0f};

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw shapes
        glBindVertexArray(VAO);

        // first triangle
        glUseProgram(shaderPrograms[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // second triangle
        glUseProgram(shaderPrograms[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(3 * sizeof(unsigned int)));

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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