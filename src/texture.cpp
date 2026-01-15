
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/texture.hpp"
#include "../include/program.hpp"
#include <string>

// up to the user to set the active texture unit
Texture::Texture(GLenum texture_type, std::vector<string> file_list, int image_width, int image_height) {
    switch(texture_type) {
        case GL_TEXTURE_2D_ARRAY:
            /* 2d texture array*/
            width = image_width;
            height = image_height;
            glGenTextures(1,&tex);
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
            int width, height, nrChannel, format;
            glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
            glActiveTexture(GL_TEXTURE0);   
            GLenum type;
            switch (nrChannel) {
                case 3:
                    type = GL_RGB;
                    break;
                case 4:
                    type = GL_RGBA;
                    break;
                default:
                    type = GL_RGB;
                    break;
            }

            glTexImage3D(GL_TEXTURE_2D_ARRAY,0,type,image_width,image_height,tex_count,0,type,GL_UNSIGNED_BYTE,NULL);
            for (int i = 0; i < tex_count; i++) {
                int x, y;
                image_ptr_list.at(i) = stbi_load(file_list.at(i),&x,&y,&format,0);
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image_width, image_height, 1, type, GL_UNSIGNED_BYTE, image_ptr_list.at(i));
            }
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
}

void Texture::setActiveTextureUnit(Shader& s, std::string uniform_name, int tex_unit) {
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    program_object.setInt(uniform_name.data(),tex_unit);
}
