#version 130

in vec4 texcoord;
uniform sampler2D myTexture;

out vec4 FragColour;

void main(){
	vec2 t_texcoord;
	if(texcoord.w < 0.0){
		t_texcoord = vec2((fract(texcoord.x) + (texcoord.w)) / 16.0, texcoord.z);  
	} else {
		t_texcoord = vec2((fract(texcoord.x + texcoord.z) + texcoord.w) / 16.0, -texcoord.y); 
	}

	FragColour = texture2D(myTexture,t_texcoord);
}
