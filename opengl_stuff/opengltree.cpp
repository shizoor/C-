#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define PI 3.14159265359

using namespace std;
// Tree version.
// Vertex Shader Source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;



out vec3 vertexColor;

void main() {
    //gl_Position = transform * vec4(aPos, 0.0, 1.0);  Use this one to rotate
    gl_Position = vec4(aPos, 0.0, 1.0);
    vertexColor = aColor;
}
)";

struct triangle2D {
    ///  x, y, r, g, b
    float vertices[15];
    void loadvalues(float inputarray[15]) {
        for (int i = 0; i < 15; i++) {
            vertices[i] = inputarray[i];
        }
    };
    void rotate(float angle) {
        float oldtriangle[6];
        float newtriangle[15];
        oldtriangle[0] = vertices[0];
        oldtriangle[1] = vertices[1];
        oldtriangle[2] = vertices[5];
        oldtriangle[3] = vertices[6];
        oldtriangle[4] = vertices[10];
        oldtriangle[5] = vertices[11];
        newtriangle[0] = oldtriangle[0] * cos(angle) - oldtriangle[1] * sin(angle);
        newtriangle[1] = oldtriangle[0] * sin(angle) + oldtriangle[1] * cos(angle);
        newtriangle[5] = oldtriangle[2] * cos(angle) - oldtriangle[3] * sin(angle);
        newtriangle[6] = oldtriangle[2] * sin(angle) + oldtriangle[3] * cos(angle);
        newtriangle[10] = oldtriangle[4] * cos(angle) - oldtriangle[5] * sin(angle);
        newtriangle[11] = oldtriangle[4] * sin(angle) + oldtriangle[5] * cos(angle);
        newtriangle[2] = vertices[2];
        newtriangle[3] = vertices[3];
        newtriangle[4] = vertices[4];
        newtriangle[7] = vertices[7];
        newtriangle[8] = vertices[8];
        newtriangle[9] = vertices[9];
        newtriangle[12] = vertices[12];
        newtriangle[13] = vertices[13];
        newtriangle[14] = vertices[14];
        for (int i = 0; i < 15; i++) {
            vertices[i] = newtriangle[i];
        }
    }
    void translate(float x, float y) {
        vertices[0] += x;
        vertices[5] += x;
        vertices[10] += x;
        vertices[1] += y;
        vertices[6] += y;
        vertices[11] += y;
    }
    void scale(float x, float y) {
        vertices[0] *= x;
        vertices[5] *= x;
        vertices[10] *= x;
        vertices[1] *= y;
        vertices[6] *= y;
        vertices[11] *= y;
    }
    glm::vec2 getTip() const {
        return glm::vec2(vertices[10], vertices[11]); // note to place the top vertex last!!!
    }
};

vector<triangle2D> trianglebuffer;


// Fragment Shader Source
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

void verticespush(float* inputvertices) {
    triangle2D mytriangle;
    mytriangle.loadvalues(inputvertices);
    trianglebuffer.push_back(mytriangle);
};

void triangle2DPush(triangle2D triangle2d) {
    trianglebuffer.push_back(triangle2d);
}


void tree(int iteration, float branchangle, float angle, float windangle, glm::vec2 position, float height, float width) {
    triangle2D firsttriangle;
    triangle2D secondtriangle;

    float red = atan(float(iteration)/20.0f);
    float green = 1.0f - atan(float(iteration) / 5.0f);

    float vertices[] = {
        // Position     // Color
        0.25f, 0.0f,    red, green, 0.0f, // bottom-left (red)
        -0.25f, 0.0f,    red, green, 0.0f, // bottom-right (green)
        -0.25f,  1.0f,   red, green, 0.0f  // top (blue)
    };
    firsttriangle.loadvalues(vertices);
    firsttriangle.scale(width, height);
    firsttriangle.rotate(angle);
    firsttriangle.translate(position.x, position.y);
    trianglebuffer.push_back(firsttriangle);
    float vertices2[] = {
        0.25f,  0.0f,   red, green, 0.0f,  
        -0.25f, 1.0f,    red, green, 0.0f, 
        0.25f, 1.0f,    red, green, 0.0f, 
    };
    secondtriangle.loadvalues(vertices2);
    secondtriangle.scale(width, height);
    secondtriangle.rotate(angle);
    secondtriangle.translate(position.x, position.y);
    trianglebuffer.push_back(secondtriangle);

    iteration--;
    if (iteration > 0) {
        glm::vec2 tip1 = firsttriangle.getTip();
        glm::vec2 tip2 = secondtriangle.getTip();
        glm::vec2 tip3;
        tip3.y = (tip1.y + tip2.y)/2;
        tip3.x = (tip1.x + tip2.x) / 2;

        float nextHeight = height * 0.7f;
        float nextWidth = width * 0.7f;

        if (iteration < 7) {
            windangle = windangle * (float(rand() % 100))/100;
        }

        // Branch left and right

        tree(iteration, branchangle, angle + branchangle + windangle , windangle, tip3, nextHeight, nextWidth);
        tree(iteration, branchangle, angle - branchangle + windangle , windangle, tip3, nextHeight, nextWidth);

        
    }

}


int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Fractal tree in the wind", nullptr, nullptr);
  
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // Enable Vsync (1 = on)
    glfwSwapInterval(1);


    // Vertex data: positions + colors
    vector<float> vertices = {
    };


    for (int i = 0; i < trianglebuffer.size(); i++) {
        for (int j = 0; j < 15; j++) {
            vertices.push_back(trianglebuffer.at(i).vertices[j]);
            cout << "pushing : " << trianglebuffer.at(i).vertices[j];
        }
        cout << endl;
    }


    // Create VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
 

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Compile Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compile Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Link Shaders into Shader Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime(); // seconds
        float angle = 0;
        float windangle = (PI / 7) * sin(time);



        // Rebuild trianglebuffer and vertices
        trianglebuffer.clear();
        vertices.clear();
        
        tree(10, 0.5f, angle, windangle, glm::vec2(0.0f, -1.0f), 0.6f, 0.5f); // Add tree segment int iteration, float angle, float xpos, float ypos, float height, float width

        for (int i = 0; i < trianglebuffer.size(); i++) {
            for (int j = 0; j < 15; j++) {
                vertices.push_back(trianglebuffer[i].vertices[j]);
                //cout << "pushing : " << trianglebuffer.at(i).vertices[j]<<endl;
                
            }
        }
        //cout << "ypos : " << 0.0f - time << endl;

        // Update VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
