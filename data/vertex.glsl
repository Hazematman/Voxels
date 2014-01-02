#version 130

in vec3 vertex;
in vec2 texcoord;
uniform mat4 mvp;

out vec2 f_texcoord;

void main(){
	gl_Position = mvp * vec4(vertex,1.0); 
	f_texcoord = texcoord;
}
