#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsUtils.hpp"
#include "Debug/DebugCube.hpp"
using namespace std;

int width, height;

bool initGL(){
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

int main(int argc, char *argv[]){
	sf::ContextSettings cs;
	cs.majorVersion = 3;
	cs.minorVersion = 0;
	cs.depthBits = 24;
	cs.stencilBits = 8;
	cs.antialiasingLevel = 4;

	width = 800; height = 600;

	sf::RenderWindow screen(sf::VideoMode(width, height), "Voxels",
			sf::Style::Default, cs);
	sf::Event event;

	GLenum glewStatus = glewInit();
	if(glewStatus != GLEW_OK){
		cerr << "Error: " << glewGetErrorString(glewStatus) << endl;
	   return EXIT_FAILURE;	
	}

	if(!GLEW_VERSION_2_0){
		cerr << "Error: OpenGL 2.x not supported" << endl;
		return EXIT_FAILURE;
	}

	if(initGL() == false){
		cerr << "Failed to initialize OpenGL" << endl;
		return EXIT_FAILURE;
	}

	ShaderProgram prg("data/vertex.glsl","data/fragment.glsl");
	prg.setAttribute("vertex");
	prg.setAttribute("texcoord");
	prg.setUniform("mvp");
	prg.setUniform("myTexture");

	//create texture
	sf::Image tex;
	tex.loadFromFile("data/cube.png");

	DebugCube cube;

	screen.setActive(true);

	sf::Clock timer;
	timer.restart();
	while(screen.isOpen()){
		while(screen.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				screen.close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				glViewport(0,0,width,height);
			}
		}
		float angle = timer.getElapsedTime().asMilliseconds() / 1000.0 * 15;
		
		glm::mat4 anim = \
			glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
			glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
			glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1)); 

		glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,
					0.0, -4.0));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), 
				glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection = glm::perspective(45.0f, 
				1.0f*width/height, 0.1f, 1000.0f);

		glm::mat4 mvp = projection * view * model * anim;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(prg.getID());
		glUniformMatrix4fv(prg.getUniform(0),1,GL_FALSE,
				glm::value_ptr(mvp));

		cube.draw(prg);
		

		screen.display();
	}
}
