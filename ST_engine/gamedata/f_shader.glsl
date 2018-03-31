#version 330 core

uniform sampler2D TexMap;
out vec4 color;
in vec2 texCoords;

void main(){
	color = texture(TexMap, texCoords, 0);
}
