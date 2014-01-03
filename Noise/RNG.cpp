#include "Noise.hpp"
using namespace std;

RNG::RNG(uint32_t seed){
	this->seed = seed;
}

RNG::RNG(){
	this->seed = time(NULL);
}

void RNG::setSeed(uint32_t seed){
	this->seed = seed;
}

uint32_t RNG::getSeed(){
	return seed;
}

uint32_t RNG::getInt(){
	seed = ((uint64_t)seed * 279470273UL) % 4294967291UL;
	return seed;	
}

int RNG::getRange(int low, int high){
	return getInt() % (high-low+1) + low;
}

float RNG::getFloat(){
	return (float)(getInt())/4294967291.0; 
}

double RNG::getDouble(){
	return (double)(getInt())/4294967291.0; 
}
