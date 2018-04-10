#ifndef TEXTURE_GL_LEGACY
#define TEXTURE_GL_LEGACY

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

typedef struct texture_gl_legacy{
    GLuint TextureID;
    int Mode;
    GLfloat width;
    GLfloat height;

    texture_gl_legacy(GLuint id, int mode, int tex_w, int tex_h){
        TextureID = id;
        Mode = mode;
        width = static_cast<GLfloat>(tex_w);
        height = static_cast<GLfloat>(tex_h);
    }

    //this will delete the object and texture from GPU memory
    ~texture_gl_legacy(){
        glDeleteTextures(1, &TextureID);
    }

} texture_gl_legacy;

#endif
