#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "shader.h"
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

using namespace std;

Camera camera;
float delta_time = 0.0f;
float last_frame = 0.0f;

void update_delta_time() {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}


void process_input(GLFWwindow* window) {


    float camera_speed = 2.5f * delta_time;


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += camera_speed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= camera_speed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += camera_speed * camera.up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.pos -= camera_speed * camera.up;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->process_mouse_movement(xpos, ypos);
    }
}

void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->process_scroll(ypos);
    }
}





struct State {
    GLFWwindow* window;
};

bool initialize(State& state) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    int width = 800;
    int height = 600;

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    state.window = window;

    glEnable(GL_DEPTH_TEST);

    return true;
}




int main() {

    State state;


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };






    if (!initialize(state))
        return -1;

    //Shader shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    Shader lighting_shader("../src/shaders/lighting.vert", "../src/shaders/lighting.frag");
    Shader light_cube_shader("../src/shaders/lighting.vert", "../src/shaders/light_cube.frag");

    unsigned int VBO, cube_VAO;
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &VBO);

    //copy the vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int light_cube_VAO;
    glGenVertexArrays(1, &light_cube_VAO);
    glBindVertexArray(light_cube_VAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);






    camera.yaw = -90.0f;

    while(!glfwWindowShouldClose(state.window)) {

        // check inputs
        update_delta_time();
        process_input(state.window);

        // render scene
        //clear the color buffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float oscillating_value1 = 2.0f * sin(3*glfwGetTime());
        float oscillating_value2 = 2.0f * cos(3*glfwGetTime());
        glm::vec3 light_pos(1.2f, oscillating_value2, oscillating_value1);



        //draw the cube
        lighting_shader.use();
        lighting_shader.set_vec3("object_color", {1.0f, 0.5f, 0.31f});
        lighting_shader.set_vec3("light_color", {1.0f, 1.0f, 1.0f});
        lighting_shader.set_vec3("light_pos", light_pos);
        lighting_shader.set_vec3("view_pos", camera.pos);

        auto view = camera.create_view_matrix();
        auto projection = camera.create_projection_matrix();
        lighting_shader.set_mat4("view", view);
        lighting_shader.set_mat4("projection", projection);

        auto model = glm::mat4(1.0f);
        lighting_shader.set_mat4("model", model);

        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);





        //draw the light
        light_cube_shader.use();
        light_cube_shader.set_mat4("projection", projection);
        light_cube_shader.set_mat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        light_cube_shader.set_mat4("model", model);

        glBindVertexArray(light_cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);




        // swap front and back buffer and call events
        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_cube_VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}