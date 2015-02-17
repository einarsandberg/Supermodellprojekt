#version 150 core

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texDog;
uniform sampler2D texLeaf;


void main() {
			//floor					//leaf
	
if (Color == vec3(1.0f,1.0f,0.9f)){
        outColor = vec4(Color, 1.0) * texture(texDog, Texcoord);
    } else {
        outColor = vec4(Color, 1.0) * texture(texLeaf, Texcoord);
    }
   
   
}