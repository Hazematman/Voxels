#ifndef NOISE_HPP
#define NOISE_HPP

#include <iostream>

class RNG {
	private:
		uint32_t seed;
	public:
		RNG(uint32_t seed);

		uint32_t getInt();
		int getRange(int low, int high);

		float getFloat();
		double getDouble();
};

#endif
