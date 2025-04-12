//
// Created by Trey Bertram on 2025-04-07.
//

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    glm::vec3 pos   = glm::vec3(0.0f, 1.0f,  3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);
    float yaw, pitch, roll;
    float last_x = 400, last_y = 300;
    float fov = 60.0f;
    bool first_mouse = true;
    glm::vec3 direction;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};


    Camera() {}

    void process_mouse_movement(double xpos, double ypos) {

        if (first_mouse)
        {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos;
        last_x = xpos;
        last_y = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }

    void process_scroll(double yoffset) {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 90.0f)
            fov = 90.0f;
    }

    glm::mat4 create_view_matrix() {
        return glm::lookAt(pos, pos + front, up);
    }

    glm::mat4 create_projection_matrix() {
        return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }

};

#endif //CAMERA_H
