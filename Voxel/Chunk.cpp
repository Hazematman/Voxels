#include <iostream>
#include <cstring>
#include "Chunk.hpp"
using namespace std;

Chunk::Chunk(){
	memset(blk,0,sizeof(blk));
	glGenBuffers(1,&vbo);
}

Chunk::~Chunk(){
	glDeleteBuffers(1,&vbo);
}

uint8_t Chunk::get(int x, int y, int z){
	return blk[x][y][z];
}

void Chunk::set(int x, int y, int z, uint8_t type){
	blk[x][y][z] = type;
	changed = true;
}

void Chunk::update(){
	changed = false;

	byte4 vertex[CX * CY * CZ * 6 * 6];
	int i = 0;
	
	for(int x = 0; x < CX; x++){
		for(int y=0;y<CY;y++){
			for(int z=0;z<CZ;z++){
				if(!blk[x][y][z])
					continue;

				// View from negative x
				if(x > 0 && !blk[x-1][y][z]){
					vertex[i++] = byte4(x,     y,     z,     blk[x][y][z]);        
					vertex[i++] = byte4(x,     y,     z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x,     y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x,     y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x,     y,     z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x,     y + 1, z + 1, blk[x][y][z]);
				}		
		 
				// View from positive x
				if(x < CX && !blk[x+1][y][z]){
					vertex[i++] = byte4(x + 1, y,     z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y,     z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y    , z + 1, blk[x][y][z]);
				}	

				// View from negative y
				if(y > 0 && !blk[x][y-1][z]){
					vertex[i++] = byte4(x, y, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y, z + 1, blk[x][y][z]+128);
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]+128);
				}

				// View from positive y
				if(y < CY && !blk[x][y+1][z]){
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]+128);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]+128);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, blk[x][y][z]+128);
				}

				// View from negative z
				if(z > 0 && !blk[x][y][z-1]){
					vertex[i++] = byte4(x, y, z, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]);
				}

				//View from positive z
				if(z < CZ && !blk[x][y][z+1]){
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, blk[x][y][z]);
				}
			}
		}
	}


	elements = i;
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,elements * 4,vertex,
			GL_STATIC_DRAW);
}

void Chunk::draw(ShaderProgram prg){
	if(changed)
		update();
	if(elements == 0)
		return;

	//cout << "DRAWING " << elements << " elements" << endl;

	glEnableVertexAttribArray(prg.getAttribute(0));
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(prg.getAttribute(0),4,GL_BYTE,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES,0,elements);

	glDisableVertexAttribArray(prg.getAttribute(0));
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
