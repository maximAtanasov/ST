/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef TEXTURE_GL
#define TEXTURE_GL

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

class texture{

	public:
	    GLuint atlas_id;
	    GLfloat width;
	    GLfloat height;
		GLfloat atlas_y;
		GLfloat atlas_x;
		bool is_on_screen = false;

	    texture(GLuint atlas_id, GLfloat atlas_x, GLfloat atlas_y, GLfloat tex_w, GLfloat tex_h){
			this->atlas_x = atlas_x;
			this->atlas_y = atlas_y;
			this->atlas_id = atlas_id;
	        width = tex_w;
	        height = tex_h;
	    }

		~texture() = default;
};

#endif
