/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

 #version 330 core

uniform sampler2D TexMap;
out vec4 color;
in vec2 texCoords;

void main(){
	color = texture(TexMap, texCoords, 0);
}
