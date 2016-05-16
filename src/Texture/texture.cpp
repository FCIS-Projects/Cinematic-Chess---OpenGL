#include "texture.h"
#include "Texture Loader/stb_image.h"

#include <iostream>
using namespace std;

Texture::Texture(const std::string& fileName, int texUnit_)
{
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
    textureUnitNumber = texUnit_;
    if(data == NULL)
                cout<< "Unable to load texture: " << fileName << endl;

    set_Texture_type(GL_TEXTURE_2D);
    set_Wrape_type(GL_REPEAT);
    set_Filter_type(GL_LINEAR);

}

Texture::~Texture()
{
        glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
        glActiveTexture(textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::set_Texture_type(GLuint Texture_type_recived)
{
        Texture_type=Texture_type_recived;
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_S, Wrape_type);
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_T, Wrape_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MIN_FILTER, Filter_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MAG_FILTER, Filter_type);

}


void Texture::set_Filter_type(GLuint Filter_type_recived)
{
        Filter_type=Filter_type_recived;
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_S, Wrape_type);
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_T, Wrape_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MIN_FILTER, Filter_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MAG_FILTER, Filter_type);

}


void Texture::set_Wrape_type(GLuint Wrape_type_recived)
{
        Wrape_type=Wrape_type_recived;
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_S, Wrape_type);
        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_T, Wrape_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MIN_FILTER, Filter_type);
    glTexParameterf(Texture_type, GL_TEXTURE_MAG_FILTER, Filter_type);

}

void Texture::setup_texture_data()
{
        glActiveTexture(textureUnitNumber);
         glGenTextures(1, &m_texture);

         glBindTexture(Texture_type, m_texture);
         if(Texture_type==GL_TEXTURE_CUBE_MAP_POSITIVE_X)
         {

                 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


         }
         else
         {
                        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_S, Wrape_type);
                        glTexParameteri(Texture_type, GL_TEXTURE_WRAP_T, Wrape_type);
                    glTexParameterf(Texture_type, GL_TEXTURE_MIN_FILTER, Filter_type);
                    glTexParameterf(Texture_type, GL_TEXTURE_MAG_FILTER, Filter_type);
         }


            glTexImage2D(Texture_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glBindTexture(Texture_type, 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 1);
            stbi_image_free(data);


}

