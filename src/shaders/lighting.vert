#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

uniform vec3 light_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;

void main() {
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    frag_pos = vec3(model * vec4(a_pos, 1.0));
    //very costly operation \/
    normal = mat3(transpose(inverse(model))) * a_normal;

    light_pos = vec3(view * vec4(light_pos, 1.0)); // transform from world space to view space
}