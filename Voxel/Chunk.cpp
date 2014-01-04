#include <iostream>
#include <cstring>
#include "Chunk.hpp"
using namespace std;

Chunk::Chunk(){
	memset(blk,0,CX*CY*CZ);//sizeof(blk));
	glGenBuffers(1,&vbo);
}

Chunk::~Chunk(){
	//TODO fix deleting buffers
	//glDeleteBuffers(1,&vbo);
}

uint8_t Chunk::get(int x, int y, int z){
	if(x < 0 || x > CX-1){
		return 0;
	}else if(y < 0 || y > CY-1){
		return 0;
	}else if(z < 0 || z > CZ-1){
		return 0;
	}
	return blk[x][y][z];
}

void Chunk::set(int x, int y, int z, uint8_t type){
	blk[x][y][z] = type;
	changed = true;
}

void Chunk::update(){
	changed = false;

	//byte4 vertex[CX * CY * CZ * 6 * 6];
	vector<byte4> vertex(CX * CY * CZ * 6 * 6);
	int i = 0;
	
	for(int x = 0; x < CX; x++){
		for(int y=0;y<CY;y++){
			for(int z=0;z<CZ;z++){
				if(!blk[x][y][z])
					continue;

				int top = 128;
				int bottom = 128;
				if(blk[x][y][z] == 2){
					top += 16;
					bottom -= 1;
				}

				// View from negative x
				if(!get(x-1,y,z)){  
					vertex[i++] = byte4(x, y, z, blk[x][y][z]);        
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);        
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);        
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]);
				}		
		 
				// View from positive x
				if(!get(x+1,y,z)){
					vertex[i++] = byte4(x + 1, y,     z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y,     z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z,     blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y + 1, z + 1, blk[x][y][z]);        
					vertex[i++] = byte4(x + 1, y    , z + 1, blk[x][y][z]);
				}	

				// View from negative y
				if(!get(x,y-1,z)){
					vertex[i++] = byte4(x, y, z, blk[x][y][z]+bottom);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]+bottom);
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]+bottom);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]+bottom);
					vertex[i++] = byte4(x + 1, y, z + 1, blk[x][y][z]+bottom);
					vertex[i++] = byte4(x, y, z + 1, blk[x][y][z]+bottom);
				}

				// View from positive y
				if(!get(x,y+1,z)){
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]+top);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]+top);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]+top);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]+top);
					vertex[i++] = byte4(x, y + 1, z + 1, blk[x][y][z]+top);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, blk[x][y][z]+top);
				}

				// View from negative z
				if(!get(x,y,z-1)){
					vertex[i++] = byte4(x, y, z, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z, blk[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, blk[x][y][z]);
				}

				//View from positive z
				if(!get(x,y,z+1)){
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
	glBufferData(GL_ARRAY_BUFFER,elements * 4,&vertex[0],
			GL_STATIC_DRAW);
}

void Chunk::draw(ShaderProgram *prg){
	if(changed)
		update();
	if(elements == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(prg->getAttribute(0),4,GL_BYTE,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES,0,elements);

}
