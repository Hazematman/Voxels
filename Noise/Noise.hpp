#ifndef NOISE_HPP
#define NOISE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>

struct Grad{
	double x,y,z,w;
	Grad(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Grad(double x, double y, double z, double w){
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

class RNG {
	private:
		uint32_t seed;
	public:
		RNG();
		RNG(uint32_t seed);

		void setSeed(uint32_t seed);
		uint32_t getSeed();

		uint32_t getInt();
		int getRange(int low, int high);

		float getFloat();
		double getDouble();
};

class Noise {
	private:
		RNG rng;
		uint8_t p[256];
		uint8_t perm[512];
		uint8_t permMod12[512];

		double F2 = 0.5*(sqrt(3.0)-1.0);
		double G2 = (3.0-sqrt(3.0))/6.0;
		double F3 = 1.0/3.0;
		double G3 = 1.0/6.0;
		double F4 = (sqrt(5.0)-1.0)/4.0;
		double G4 = (5.0-sqrt(5.0))/20.0;

		int fastFloor(double x);

		double dot(Grad g, double x, double y);
		double dot(Grad g, double x, double y, double z);
		double dot(Grad g, double x, double y, double z, double w);

		double noise2D(double x, double y);
		double noise3D(double x, double y, double z);
		double noise4D(double x, double y, double z, double w);
	public:
		Noise(uint32_t seed);

		double get(double x);
		double get(double x, double y);
		double get(double x, double y, double z);
		double get(double x, double y, double z, double w);
};

#endif
