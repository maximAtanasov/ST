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
