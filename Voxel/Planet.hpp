#ifndef PLANET_HPP
#define PLANET_HPP

#include <iostream>
#include <vector>
#include "Chunk.hpp"
#include "../Noise/Noise.hpp"

class Planet {
	private:
		std::vector<Chunk> chunks;
		int size;
		
		Noise noise;

		void setChunk(int x,int y);
	public:
		Planet(int size);

		void draw(ShaderProgram *prg);
		Chunk *getChunk(int x, int y);
};

#endif
