#version 130

in vec4 texcoord;
uniform sampler2D myTexture;

out vec4 FragColour;

void main(){
	vec2 t_texcoord;
	if(texcoord.w < 0.0){
		float yfactor = (0.0625)*floor((texcoord.w+128)/16.0);
		t_texcoord = vec2((fract(texcoord.x) + (texcoord.w)) / 16.0, (fract(texcoord.z)/16.0)+yfactor);  
	} else {
		float yfactor = (0.0625)*floor((texcoord.w/16.0));
		t_texcoord = vec2((fract(texcoord.x + texcoord.z) + texcoord.w) / 16.0, (fract(-texcoord.y) / 16.0)+yfactor); 
	}

	FragColour = texture2D(myTexture,t_texcoord);
}
