#version 130

in vec4 coord;
uniform mat4 m;
uniform mat4 vp;

out vec4 texcoord;

void main(){
	texcoord = coord;
	gl_Position = vp * m * vec4(coord.xyz, 1);
}
