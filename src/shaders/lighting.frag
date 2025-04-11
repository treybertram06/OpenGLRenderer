#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos_trans;
in vec2 tex_coords;

uniform Material material;
uniform Light light;

out vec4 frag_color;

void main() {
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));

    //diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos_trans - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));

    //specular
    vec3 view_dir = normalize(-frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}