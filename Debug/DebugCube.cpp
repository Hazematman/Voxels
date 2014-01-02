#include <cstring>
#include <SFML/Graphics.hpp>
#include "DebugCube.hpp"
using namespace std;

GLfloat cubeVerts[] = {
	// front
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// top
	-1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
	// back
	 1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	 1.0,  1.0, -1.0,
	// bottom
	-1.0, -1.0, -1.0,
	 1.0, -1.0, -1.0,
	 1.0, -1.0,  1.0,
	-1.0, -1.0,  1.0,
	// left
	-1.0, -1.0, -1.0,
	-1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	// right
	 1.0, -1.0,  1.0,
	 1.0, -1.0, -1.0,
	 1.0,  1.0, -1.0,
	 1.0,  1.0,  1.0,
};

GLfloat cubeColours[] = {
	// front
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	// top
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	// back
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,
	// bottom
	 1.0,  1.0,  0.0,
	 1.0,  1.0,  0.0,
	 1.0,  1.0,  0.0,
	 1.0,  1.0,  0.0,
	// left
	 1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	// right
	 0.0,  1.0,  1.0,
	 0.0,  1.0,  1.0,
	 0.0,  1.0,  1.0,
	 0.0,  1.0,  1.0,
};

GLushort cubeElements[] = {
	// front
	 0,  1,  2,
	 2,  3,  0,
	// top
	 4,  5,  6,
	 6,  7,  4,
	// back
	 8,  9, 10,
	10, 11,  8,
	// bottom
	12, 13, 14,
	14, 15, 12,
	// left
	16, 17, 18,
	18, 19, 16,
	// right
	20, 21, 22,
	22, 23, 20,
};

DebugCube::DebugCube(){
	GLfloat cubeTexcoords[2*4*6] = {
	// front
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	};
	for(int i=1;i<6;i++){
		memcpy(&cubeTexcoords[i*4*2],&cubeTexcoords[0],2*4*sizeof(GLfloat));
	}

	glGenBuffers(1,&tex);
	glBindBuffer(GL_ARRAY_BUFFER,tex);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeTexcoords),cubeTexcoords,
			GL_STATIC_DRAW);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts),cubeVerts,
			GL_STATIC_DRAW);

	glGenBuffers(1,&cbo);
	glBindBuffer(GL_ARRAY_BUFFER,cbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeColours),cubeColours,
			GL_STATIC_DRAW);

	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeElements),
			cubeElements,GL_STATIC_DRAW);

	//create texture
	sf::Image tex;
	tex.loadFromFile("data/cube.png");
	glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			tex.getSize().x,
			tex.getSize().y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			tex.getPixelsPtr());	
}

DebugCube::~DebugCube(){
	glDeleteBuffers(1,&vbo);
	glDeleteBuffers(1,&ibo);
	glDeleteBuffers(1,&cbo);
	glDeleteBuffers(1,&tex);
	glDeleteTextures(1,&texID);
}

void DebugCube::draw(ShaderProgram prg){

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(prg.getUniform(1),0);

	glEnableVertexAttribArray(prg.getAttribute(0));
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(
			prg.getAttribute(0),
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);

	glEnableVertexAttribArray(prg.getAttribute(1));
	glBindBuffer(GL_ARRAY_BUFFER,tex);
	glVertexAttribPointer(
			prg.getAttribute(1),
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glDrawElements(GL_TRIANGLES, sizeof(cubeElements),GL_UNSIGNED_SHORT,0);

	glDisableVertexAttribArray(prg.getAttribute(0));
	glDisableVertexAttribArray(prg.getAttribute(1));
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
