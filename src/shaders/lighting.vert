#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;


uniform vec3 light_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;
out vec3 light_pos_trans;
out vec2 tex_coords;

void main() {
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    frag_pos = vec3(view * model * vec4(a_pos, 1.0));
    //very costly operation \/
    normal = mat3(transpose(inverse(view * model))) * a_normal;

    light_pos_trans = vec3(view * vec4(light_pos, 1.0)); // transform from world space to view space
    tex_coords = a_tex_coords;
}