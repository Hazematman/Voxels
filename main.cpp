#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <time.h>
#include "Globals.hpp"
#include "GraphicsUtils.hpp"
#include "FPSCamera.hpp"
#include "Noise/Noise.hpp"
#include "Debug/DebugCube.hpp"
#include "Voxel/Planet.hpp"
using namespace std;

int width, height;
RNG randoms;
Noise gNoise(time(NULL));

bool initGL(){
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	return true;
}

int main(int argc, char *argv[]){
	cout << "Random Seed is " << randoms.getSeed() << endl;
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

	shared_ptr<ShaderProgram> prg(new ShaderProgram("data/vertex.glsl","data/fragment.glsl"));
	prg->setAttribute("vertex");
	prg->setAttribute("texcoord");
	prg->setUniform("mvp");
	prg->setUniform("myTexture");

	shared_ptr<ShaderProgram> vprg(new ShaderProgram("data/shaders/voxelV.glsl",
			"data/shaders/voxelF.glsl"));
	vprg->setAttribute("coord");
	vprg->setUniform("vp");
	vprg->setUniform("m");
	vprg->setUniform("myTexture");

	DebugCube cube;

	Planet p(4);

	//Load texture atlas

	sf::Image texAt;
	texAt.loadFromFile("data/textures/terrain.png");

	GLuint texID;
	glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			texAt.getSize().x,
			texAt.getSize().y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texAt.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	screen.setActive(true);

	FPSCamera camera(-3,0,-4,0,0);

	sf::Clock timer;
	sf::Time dt;
	timer.restart();
	while(screen.isOpen()){
		while(screen.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				screen.close();
			}
			if(event.key.code == sf::Keyboard::Escape){
				screen.close();
			}
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				glViewport(0,0,width,height);
			}
		}


		//Change camera stuffam
		screen.setMouseCursorVisible(false);
		sf::Vector2i pos = sf::Mouse::getPosition(screen) - 
			sf::Vector2i(width/2,height/2);
		camera.turnX(pos.x*dt.asSeconds()*50.0);
		camera.turnY(pos.y*dt.asSeconds()*50.0);
		sf::Mouse::setPosition(sf::Vector2i(width/2,height/2),
				screen);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			camera.move(10*dt.asSeconds());
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			camera.move(-10*dt.asSeconds());
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			camera.strafe(-10*dt.asSeconds());
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			camera.strafe(10*dt.asSeconds());
		}

		//Create projection matrix and view-project matrix;
		glm::mat4 projection = glm::perspective(45.0f, 
				1.0f*width/height, 0.1f, 1000.0f);

		glm::mat4 mvp = projection * camera.view();

		glm::mat4 cubem = glm::translate(glm::mat4(1.0),
				glm::vec3(-3,0,0));

		glm::mat4 cmvp = mvp*cubem;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(prg->getID());
		glUniformMatrix4fv(prg->getUniform(0),1,GL_FALSE,
				glm::value_ptr(cmvp));

		cube.draw(prg.get());

		glUseProgram(vprg->getID());
		glUniformMatrix4fv(vprg->getUniform(0),1,GL_FALSE,
				glm::value_ptr(mvp));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texID);
		glUniform1i(vprg->getUniform(2),0);

		p.draw(vprg.get());

		screen.display();
		dt = timer.restart();
	}
}
