/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#version 330 core


uniform float x;
uniform float y;
uniform float scale_x;
uniform float scale_y;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
out vec2 texCoords;


void main(){
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0f;
	gl_Position.x = gl_Position.x;
	gl_Position.y = gl_Position.y;
	texCoords = vertexUV;
}
