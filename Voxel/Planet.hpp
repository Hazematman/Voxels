#ifndef PLANET_HPP
#define PLANET_HPP

#include <iostream>
#include <vector>
#include "Chunk.hpp"

class Planet {
	private:
		std::vector<std::vector<Chunk>> chunks;
	public:
		Planet(int size);
};

#endif
