#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static unsigned int CompileShader(const std::string& source, unsigned int type);
static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader);
static struct ShaderProgramSource ParseShader(const std::string& filepath);

void keycallback(GLFWwindow* window, int pressed_key, int syst_spec_key,
                 int key_state, int mode_bit_field) {
    if (pressed_key == GLFW_KEY_ESCAPE) {
        std::cout << "Pressed" << std::endl;
        glfwSetWindowShouldClose(window, 1);
    }
}

int main() {
    GLFWwindow* window;

    // Initializing the library
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keycallback);
    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

    // Print the version of OPENGL
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5, -0.5f, 0.5, -0.5f, 0.5, 0.5f, -0.5, 0.5f,
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    // create a handle called buffer to handle the position buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // Bind this buffer to current context
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions,
                 GL_STATIC_DRAW);

    // By default the Vertex array attributes are disabled so we need to enable
    // them
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices,
                 GL_STATIC_DRAW);  // Has to be unsigned ( be it char or int)

    struct ShaderProgramSource shaders =
        ParseShader("../res/shaders/Basic.shader");
    /*
     *std::cout << "VERTEX:" << "\n";
     *std::cout << shaders.VertexSource << "\n";
     *std::cout << "FRAGMENT:" << "\n";
     *std::cout << shaders.FragmentSource << "\n";
     */

    unsigned int shader =
        CreateShader(shaders.VertexSource, shaders.FragmentSource);
    glUseProgram(shader);
    // Set the background color
    glClearColor(0.8, 0.2, 0.2, 1.0);

    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        // Use the  buffer bound to current context to draw the triangle
        glDrawElements(
            GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT,
            nullptr);  // total of 2 triangle with 3 positions per triangle
        // Swap front and backbuffer
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    std::cout << "Window Closed";
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

static unsigned int CompileShader(unsigned int type,
                                  const std::string& source) {
    // Get a handle for our vertex shader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    const int length = strlen(src);
    glShaderSource(id, 1, &src, &length);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                  << " shader!\n";
        std::cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

static struct ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int)type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}
