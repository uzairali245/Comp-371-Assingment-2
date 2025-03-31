#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>

struct Vertex { float x, y, z; };
std::vector<Vertex> vertices;
std::vector<unsigned int> indices;
glm::vec3 translation(0.0f, 0.0f, 0.0f);
glm::vec3 scale(1.0f, 1.0f, 1.0f);      // Larger initial scale
float rotationAngle = 0.0f;

void loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return;
    }
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (type == "f") {
            std::vector<std::string> faceTokens;
            std::string token;
            while (iss >> token) {
                faceTokens.push_back(token);
            }
            if (faceTokens.size() < 3) {
                std::cerr << "Line " << lineNum << ": Face has fewer than 3 vertices: " << line << std::endl;
                continue;
            }
            for (int i = 0; i < 3; i++) {
                std::string& token = faceTokens[i];
                size_t firstSlash = token.find('/');
                if (firstSlash == std::string::npos) {
                    unsigned int idx = std::stoi(token) - 1;
                    indices.push_back(idx);
                } else {
                    unsigned int idx = std::stoi(token.substr(0, firstSlash)) - 1;
                    indices.push_back(idx);
                }
            }
        }
    }
    file.close();
    std::cout << "Loaded " << vertices.size() << " vertices and " << indices.size() / 3 << " triangles." << std::endl;
    std::cout << "Total indices: " << indices.size() << std::endl;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translation.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translation.y -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translation.x -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translation.x += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        scale *= 1.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        scale *= 0.99f;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotationAngle += 1.0f;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(800, 600, "Cybertruck Assignment", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        std::cerr << "Failed to get working directory" << std::endl;
    }

    loadOBJ("Cybertruck.obj");
    glEnable(GL_DEPTH_TEST);

    // Set up projection and view matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 20.0f), // Camera farther back
            glm::vec3(0.0f, 0.0f, 0.0f),  // Look at origin
            glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
    );

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        glm::mat4 mvp = projection * view * model;
        glLoadMatrixf(&mvp[0][0]);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (indices.empty()) {
            std::cerr << "No indices to render!" << std::endl;
        } else if (indices.size() % 3 != 0) {
            std::cerr << "Invalid number of indices: " << indices.size() << " (must be multiple of 3)" << std::endl;
        } else {
            glBegin(GL_TRIANGLES);
            for (unsigned int i : indices) {
                if (i >= vertices.size()) {
                    std::cerr << "Invalid index: " << i << " (max: " << vertices.size() - 1 << ")" << std::endl;
                    glEnd();
                    break;
                }
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
            }
            glEnd();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}