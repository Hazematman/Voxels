#ifndef DEBUGCUBE_HPP
#define DEBUGCUBE_HPP

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "../GraphicsUtils.hpp"

class DebugCube {
	private:
		GLuint vbo;
		GLuint ibo;
		GLuint cbo;
		GLuint tex;

		GLuint texID;
	public:
		DebugCube();
		~DebugCube();

		void draw(ShaderProgram prg);
};

#endif
