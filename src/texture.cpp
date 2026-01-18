
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/texture.hpp"
#include "../include/shader.hpp"
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// up to the user to set the active texture unit
Texture::Texture(GLenum texture_type, std::vector<std::string> &file_list, int image_width, int image_height, int active_unit) {
    switch(texture_type) {
        case GL_TEXTURE_2D_ARRAY:
            /* 2d texture array*/
            width = image_width;
            height = image_height;
            glGenTextures(1,&tex);
            glActiveTexture(GL_TEXTURE0 + active_unit);   
            glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
            glTexParameteri(GL_TEXTURE_2D_ARRAY,
                        GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY,
                        GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
            int tex_count = file_list.size();
            std::vector<unsigned char*> image_ptr_list(tex_count,nullptr);
            int width, height, format;
            GLenum type = GL_RGBA;


            glTexImage3D(GL_TEXTURE_2D_ARRAY,0,type,image_width,image_height,tex_count,0,type,GL_UNSIGNED_BYTE,NULL);
            for (int i = 0; i < tex_count; i++) {
                int x, y, format;
                image_ptr_list.at(i) = stbi_load(file_list.at(i).data(),&x,&y,&format,0);
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image_width, image_height, 1, type, GL_UNSIGNED_BYTE, image_ptr_list.at(i));
            }
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
            glActiveTexture(GL_TEXTURE0);   
    }
}

void Texture::bindToTextureUnit(int tex_unit) {
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
}
