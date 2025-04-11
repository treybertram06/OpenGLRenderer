#version 330 core
out vec4 frag_color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dir_light;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight point_lights[NR_POINT_LIGHTS];

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coords;

uniform vec3 view_pos;
uniform Material material;

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main() {

    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    //directional lighting
    vec3 result = calc_dir_light(dir_light, norm, view_dir);

    //point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
            result += calc_point_light(point_lights[i], normal, frag_pos, view_dir);

    //spot light(s?)

    frag_color = vec4(result, 1.0);

}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);
    //diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    //specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    //combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));
    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_pos);
    //diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    //specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    //attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));
    ambient  *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

