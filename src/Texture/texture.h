#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

//This class handel the textures of the project by binding,generating and allocating the data


class Texture
{

public:
	/*unsigned*/ int width, height; //the variables in which the texture loader will send the image info
	int numComponents; //the variable in which the texture loader will send the image info
	unsigned char * data;//the variables in which save the data and path
	int textureUnitNumber;//texture Unit number
	void Bind(); //it activates the texture unite and bind the texture id
	Texture(const std::string& fileName, int texUnit);//constaractor by3ml ay 7aga w kol 7aga lamo2a5za
	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
	GLuint m_texture;
};

#endif
//GLuint loadBMP_custom(const char * imagepath);
