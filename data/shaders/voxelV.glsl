#version 130

in vec4 coord;
uniform mat4 mvp;

out vec4 texcoord;

void main(){
	texcoord = coord;
	gl_Position = mvp * vec4(coord.xyz, 1);
}