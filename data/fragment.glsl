#version 130

in vec2 f_texcoord;
uniform sampler2D myTexture;

out vec4 fragColour;

void main(){
	vec2 flipped = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
	fragColour = texture2D(myTexture,flipped);
}
