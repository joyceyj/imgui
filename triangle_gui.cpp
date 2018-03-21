
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GL/gl3w.h"
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// size of the window
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"    vertColor = color;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"in vec3 vertColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertColor, 1.0f);\n"
"}\n\0";

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow* window);

int main() {
    // initial the glfw window
    glfwInit();
    // set the argument
    // use opengl3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window and name the title of "triangle"
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HIGHT, "Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create the window!" << std::endl;
        // terminate
        glfwTerminate();
        return -1;
    }
    // confirm GLFW to make the context as the main context of current  process
    glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSwapInterval(1);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    // // glad
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check the shader compile
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED/N" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Fragment::COMPILATION_FAILED/N" << infoLog << std::endl;
    }

    // link shader
    int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check the shader program link
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Program::COMPILATION_FAILED/N" << infoLog << std::endl;
    }
    // delete shader affter link program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    // vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // 解除绑定VBO到内存
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // setup ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);

    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 my_color1 = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
    ImVec4 my_color2 = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
    ImVec4 my_color3 = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
    bool showToolBar = true;
    bool basic3 = true;
    bool basic2 = false;
    bool basic1 = false;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        // processInput(window);

        // check the events called
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        // clear the color buffer
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if (showToolBar) {
            ImGui::Begin("Tool Bar", &showToolBar);
            ImGui::Text("change the clear color!");
            ImGui::ColorEdit4("Clear Color", (float*)&clear_color);
            ImGui::Checkbox("Basic3", &basic3);
            ImGui::Checkbox("Basic2", &basic2);
            ImGui::Checkbox("Basic1", &basic1);
            if (basic3) {
                ImGui::Text("change the triangle color!");
                ImGui::ColorEdit4("vertex1Color", (float*)&my_color1);
                ImGui::ColorEdit4("vertex2Color", (float*)&my_color2);
                ImGui::ColorEdit4("vertex3Color", (float*)&my_color3);
                vertices[3] = my_color1.x;
                vertices[9] = my_color2.x;
                vertices[15] = my_color3.x; 
                vertices[4] = my_color1.y;
                vertices[10] = my_color2.y;
                vertices[16] = my_color3.y;
                vertices[5] = my_color1.z;
                vertices[11] = my_color2.z;
                vertices[17] = my_color3.z;
            }
            if (basic2) {
                vertices[3] = 1.0f;
                vertices[9] = 0.0f;
                vertices[15] = 0.0f; 
                vertices[4] = 0.0f;
                vertices[10] = 1.0f;
                vertices[16] = 0.0f;
                vertices[5] = 0.0f;
                vertices[11] = 0.0f;
                vertices[17] = 1.0f;            	
            }
            if (basic1) {
            	vertices[3] = 1.00f;
                vertices[9] = 1.00f;
                vertices[15] = 1.00f; 
                vertices[4] =  0.5f;
                vertices[10] = 0.5f;
                vertices[16] = 0.5f;
                vertices[5] = 0.2f;
                vertices[11] = 0.2f;
                vertices[17] = 0.2f;
            }
            ImGui::End();            
        }
        

        
        // render
        glUseProgram(shaderProgram);
        // draw triangle
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffer
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

// void processInput(GLFWwindow* window) {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//         glfwSetWindowShouldClose(window, true);
//     }
// }
