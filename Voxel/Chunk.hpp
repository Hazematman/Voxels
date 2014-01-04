#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../GraphicsUtils.hpp"

#define CX 16
#define CY 256
#define CZ 16

typedef glm::detail::tvec4<GLbyte> byte4;

class Chunk {
	private:
		uint8_t blk[CX][CY][CZ];
		GLuint vbo;
		int elements;
		bool changed;

		uint8_t getType(int x, int y, int z);
	public:
		Chunk();
		~Chunk();

		uint8_t get(int x, int y, int z);
		void set(int x, int y, int z, uint8_t type);

		void update();
		void draw(ShaderProgram *prg);	
};

#endif
