#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Planet.hpp"
#include "../Globals.hpp"
using namespace std;

double fNoiseScale = 0.006;

Planet::Planet(int size):noise(randoms.getInt()){
	this->size = size;
	chunks.resize(size*size);

	for(int y=0;y<size;y++){
		for(int x=0;x<size;x++){
			setChunk(x,y);
		}
	}

	getChunk(3,3)->set(0,0,0,6);
}

Chunk *Planet::getChunk(int x, int y){
	return &chunks.at(y*size+x);
}

void Planet::setChunk(int x, int y){
	double realSize = size*16.0;
	double fNX = 0;
	double fNY = 0;
	double fRDX = 0;
	double fRDY = 0;
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	for(int i=0;i<16;i++){
		fNY = (i+(y*16))/realSize;
		fRDY = fNY*2*PI;
		c = realSize*sin(fRDY);
		d = realSize*cos(fRDY);
		for(int j=0;j<16;j++){
			//Compute hight value
			fNX = (j+(x*16))/realSize;
			fRDX = fNX*2*PI;
			a = realSize*sin(fRDX);
			b = realSize*cos(fRDX);

			double v = gNoise.get(
					123+a*fNoiseScale,
					231+b*fNoiseScale,
					312+c*fNoiseScale,
					273+d*fNoiseScale);

			int height = (v*15.0)+15.0;
			height += 60;

			bool dirt = false;
			for(int k=0;k<128;k++){
				if(k  >= (height - 4)){
					dirt = true;
				} else if( k > height){
					dirt = false;
				}

				if(k == height){
					getChunk(x,y)->set(j,k,i,2);
				} else if(k > height){
					getChunk(x,y)->set(j,k,i,0);
				} else if(dirt){
					getChunk(x,y)->set(j,k,i,1);
				} else {
					getChunk(x,y)->set(j,k,i,3);
				} 
			}
		}
	}
}

void Planet::draw(ShaderProgram *prg){
	glEnableVertexAttribArray(prg->getAttribute(0));
	for(int y=0;y<size;y++){
		for(int x=0;x<size;x++){
			glm::mat4 model = glm::translate(glm::mat4(1.0f),
					glm::vec3(x*16.0,-80,y*16.0));
			//Move model matrix so they don't overlap
			glUniformMatrix4fv(prg->getUniform(1),1,GL_FALSE,
					glm::value_ptr(model));
			//Draw chunk
			//chunks[x][y].draw(prg);
			getChunk(x,y)->draw(prg);
		}
	}
	glDisableVertexAttribArray(prg->getAttribute(0));
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
