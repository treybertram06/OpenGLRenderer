//
// Created by treyb on 2025-04-10.
//

#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H

#include <glad/glad.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int load_texture(
    const char* path,
    GLint wrap_s = GL_MIRRORED_REPEAT,
    GLint wrap_t = GL_MIRRORED_REPEAT,
    GLint min_filter = GL_LINEAR_MIPMAP_LINEAR,
    GLint mag_filter = GL_LINEAR
)
{
    // Generate and bind a texture object
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    // Load the image data using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // optional if you want the image flipped
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        // Determine the image format
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unexpected number of channels: " << nrChannels << "\n";
            format = GL_RGB; // fallback
        }

        // Upload data to the currently bound texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free the image memory
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    // Return the texture object ID
    return textureID;
}

#endif //UTIL_FUNCS_H
