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
	Texture(const std::string& fileName, int texUnit);//constaractor 
	virtual ~Texture();
protected:
private:
	GLuint m_texture;//the id of the texture
	GLuint Texture_type;
	GLuint Wrape_type;
	GLuint Filter_type;
	Texture(const Texture& texture);
	void operator=(const Texture& texture);

	void set_Texture_type(GLuint Texture_type_recived);
	void set_Wrape_type(GLuint Wrape_type_recived);
	void set_Filter_type(GLuint Filter_type_recived);
	void setup_texture_data();


};

#endif

