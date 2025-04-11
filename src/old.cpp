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

    return true;
}




int main() {

    State state;


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);


    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    if (!initialize(state))
        return -1;

    Shader shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    Shader lighting_shader("../src/shaders/lighting.vert", "../src/shaders/lighting.frag");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //copy the vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    //set the vertex attributes pointers
    //position attribute
    /*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //textures

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &texture2);
    //set texture wrapping / filtering options on currently bound texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;
    unsigned char *data = stbi_load("../assets/wall.jpg", &width, &height, &nr_channels, 0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        clog << "Failed to load texture.\n";
    }
    stbi_image_free(data);

    unsigned char *data2 = stbi_load("../assets/cardboard.jpg", &width, &height, &nr_channels, 0);
    glBindTexture(GL_TEXTURE_2D, texture2);
    if (data2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        clog << "Failed to load texture.\n";
    }
    stbi_image_free(data2);

    lighting_shader.use();
    lighting_shader.set_vec3("object_color", {1.0f, 0.5f, 0.31f});
    lighting_shader.set_vec3("light_color", {1.0f, 1.0f, 1.0f});


    shader.use();
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);


    // vector!!!!
/*
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    std::cout << vec.x << vec.y << vec.z << std::endl;

    unsigned int transform_loc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
*/


    glEnable(GL_DEPTH_TEST);

    glm::mat4 view;
    camera.yaw = -90.0f;

    while(!glfwWindowShouldClose(state.window)) {

        // check inputs
        update_delta_time();
        process_input(state.window);

        // render scene
        //clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

/*
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        int model_loc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
*/
        view = camera.create_view_matrix();
        int view_loc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));


        auto projection = camera.create_projection_matrix();
        int projection_loc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));





        /*float time_val = glfwGetTime();
        float green_val = (sin(time_val) / 2.0f) + 0.5f;
        int vertex_color_loc = glGetUniformLocation(shader.ID, "our_color");
        glUniform4f(vertex_color_loc, 0.0f, green_val, 0.0f, 1.0f);*/

        // render triangles
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            if (i%3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // if using EBO of indices

        // swap front and back buffer and call events
        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }



    glfwTerminate();
    return 0;
}