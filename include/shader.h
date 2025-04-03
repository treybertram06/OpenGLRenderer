//
// Created by treyb on 2025-03-24.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"

class Shader {
public:
    //program id
    unsigned int ID;

    Shader(const char* vertex_path, const char* fragment_path) {
        std::string vertex_code, fragment_code;
        std::ifstream v_shader_file, f_shader_file;

        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);
            std::stringstream v_shader_stream, f_shader_stream;

            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();

            v_shader_file.close();
            f_shader_file.close();

            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        } catch (std::ifstream::failure e) {
            std::clog << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = fragment_code.c_str();

        unsigned int vertex, fragment;
        int success;
        char info_log[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    //use the shader
    void use() {
        glUseProgram(ID);
    }

    //util functions
    void set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void set_int(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set_float(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_mat4(const std::string &name, const glm::mat4 &value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
};

#endif //SHADER_H
