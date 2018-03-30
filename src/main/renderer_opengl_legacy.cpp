#include "renderer_opengl_legacy.hpp"

int renderer_opengl_legacy::initialize(SDL_Window* window, int v_width, int v_height){
    gFont_cache = new font_cache_opengl(100);

    //initialize renderer
    this->window = window;
    gl_contex = SDL_GL_CreateContext(window);

    // Let's start by clearing the whole screen with black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Both these values must be your real window size, so of course these values can't be static
    int screen_width = 0;
    int screen_height = 0;
	SDL_GetWindowSize(window, &screen_width, &screen_height);

    // This is your target virtual resolution for the game, the size you built your game to
    int virtual_width=v_width;
    int virtual_height=v_height;
	w_width = v_width;
	w_height = v_height;

    float targetAspectRatio = virtual_width/(float)virtual_height;

    // figure out the largest area that fits in this resolution at the desired aspect ratio
    int width = screen_width ;
    auto height = static_cast<int>(lround(width / targetAspectRatio + 0.5));

    if (height > screen_height ){
       //It doesn't fit our height, we must switch to pillarbox then
        height = screen_height ;
        width = static_cast<int>(lround(height * targetAspectRatio + 0.5));
    }

    // set up the new viewport centered in the backbuffer
    int vp_x = (screen_width  / 2) - (width / 2);
    int vp_y = (screen_height / 2) - (height/ 2);

    SDL_GL_MakeCurrent(this->window, gl_contex);
    glViewport(vp_x,vp_y,width,height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, screen_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    float scale_x = (float)(screen_width) / ((float)virtual_width);
    float scale_y = (float)(screen_height) / ((float)virtual_height);
    glScalef(scale_x, scale_y, 1.0f);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable (GL_ARB_texture_non_power_of_two);
    glEnable(GL_TEXTURE_2D);
    char *versionGL;
    versionGL = (char *)(glGetString(GL_VERSION));
    printf("OpenGL Version: %c\n", *versionGL);
    return 0;
}

void renderer_opengl_legacy::present(){
    SDL_GL_SwapWindow(window);
}

void renderer_opengl_legacy::draw_texture(size_t arg, int x, int y){
    texture_gl_legacy* Texture = textures[arg];
    draw_texture_(Texture, x, y - static_cast<int>(Texture->height));
}

void renderer_opengl_legacy::draw_rectangle(int x, int y, int w, int h, SDL_Color color){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLfloat col = 1.0f/255.0f;
    GLfloat r = col*(GLfloat)color.r, g = col*(GLfloat)color.g, b = col*(GLfloat)color.b, a = col*(GLfloat)color.a;
    auto X = static_cast<GLfloat>(x), Y = static_cast<GLfloat>(y), W = static_cast<GLfloat>(w), H = static_cast<GLfloat>(h);
    GLfloat max_x = X + W;
    GLfloat max_y = Y + H;
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(r, g, b, a);
    glVertex2f(X, Y);
    glVertex2f(max_x, Y);
    glVertex2f(X, max_y);
    glVertex2f(max_x, max_y);
    glColor4f(1, 1, 1, 1);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderer_opengl_legacy::draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color){
	GLfloat col = 1.0f/255.0f;
	GLfloat r = col*(GLfloat)color.r, g = col*(GLfloat)color.g, b = col*(GLfloat)color.b, a = col*(GLfloat)color.a;
	auto X = static_cast<GLfloat>(x), Y = static_cast<GLfloat>(y), W = static_cast<GLfloat>(w), H = static_cast<GLfloat>(h);
	GLfloat max_x = X + W;
	GLfloat max_y = Y + H;
	glBegin(GL_TRIANGLE_STRIP);
		glColor4f(r, g, b, a);
		glVertex2f(X, Y);
		glVertex2f(max_x, Y);
		glVertex2f(X, max_y);
		glVertex2f(max_x, max_y);
		glColor4f(1, 1, 1, 1);
	glEnd();
}

void renderer_opengl_legacy::draw_background(size_t arg){
    texture_gl_legacy* Texture = textures[arg];
    if(Texture != nullptr){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
		GLfloat Width = w_width;
		GLfloat Height = w_height;
		glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0, 0); glVertex2f(0, 0);
            glTexCoord2f(1, 0); glVertex2f(Width, 0);
            glTexCoord2f(0, 1); glVertex2f(0, Height);
            glTexCoord2f(1, 1); glVertex2f(Width, Height);
        glEnd();
	    glDisable(GL_TEXTURE_2D);
    }
}

void renderer_opengl_legacy::draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num){
    texture_gl_legacy* Texture = textures[arg];
    if(Texture != nullptr){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
        auto Width = static_cast<int>(Texture->width);
        auto Height = static_cast<int>(Texture->height);
        int temp1 = Height/animation_num;
        int temp2 = Width/sprite_num;
        GLfloat minx, miny, maxx, maxy;
        GLfloat minu, maxu, minv, maxv;
        SDL_Rect dstrect = {x, y - temp1, temp2, temp1};
        SDL_Rect srcrect = {sprite*(Width/sprite_num), temp1*(animation-1), temp2, temp1};

        minx = (GLfloat)dstrect.x;
        miny = (GLfloat)dstrect.y;
        maxx = (GLfloat)dstrect.x + (GLfloat)dstrect.w;
        maxy = (GLfloat)dstrect.y + (GLfloat)dstrect.h;

        minu = (GLfloat) srcrect.x / Width;
        maxu = (GLfloat) (srcrect.x + srcrect.w) / Width;
        minv = (GLfloat) srcrect.y / Height;
        maxv = (GLfloat) (srcrect.y + srcrect.h) / Height;

        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(minu, minv);
            glVertex2f(minx, miny);
            glTexCoord2f(maxu, minv);
            glVertex2f(maxx, miny);
            glTexCoord2f(minu, maxv);
            glVertex2f(minx, maxy);
            glTexCoord2f(maxu, maxv);
            glVertex2f(maxx, maxy);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void renderer_opengl_legacy::draw_overlay(size_t arg, int sprite, int sprite_num){
    texture_gl_legacy* Texture = textures[arg];
    if(Texture != nullptr){
        glEnable(GL_TEXTURE_2D);
        int animation_num = 1;
        int animation = 1;
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
        auto Width = static_cast<int>(Texture->width);
        auto Height = static_cast<int>(Texture->height);
        int temp1 = Height/animation_num;
        int temp2 = Width/sprite_num;
        GLfloat minx, miny, maxx, maxy;
        GLfloat minu, maxu, minv, maxv;

        SDL_Rect dstrect = {0, 0, w_width, w_height};
        SDL_Rect srcrect = {sprite*(Width/sprite_num), temp1*(animation-1), temp2, temp1};

        minx = (GLfloat)dstrect.x;
        miny = (GLfloat)dstrect.y;
        maxx = (GLfloat)dstrect.x + (GLfloat)dstrect.w;
        maxy = (GLfloat)dstrect.y + (GLfloat)dstrect.h;

        minu = (GLfloat) srcrect.x / Width;
        maxu = (GLfloat) (srcrect.x + srcrect.w) / Width;
        minv = (GLfloat) srcrect.y / Height;
        maxv = (GLfloat) (srcrect.y + srcrect.h) / Height;

        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(minu, minv);
            glVertex2f(minx, miny);
            glTexCoord2f(maxu, minv);
            glVertex2f(maxx, miny);
            glTexCoord2f(minu, maxv);
            glVertex2f(minx, maxy);
            glTexCoord2f(maxu, maxv);
            glVertex2f(maxx, maxy);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void renderer_opengl_legacy::clear_screen(){
    glClearColor(0,0,0,1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Text fallback for non-english text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
void renderer_opengl_legacy::draw_text_normal(std::string arg, std::string arg2, int x, int y, SDL_Color color, int size){
    std::string font_and_size = arg+std::to_string(size);
    TTF_Font* font = fonts[font_and_size];
    if(font != nullptr){
        if(gFont_cache->str_is_cached(arg2, arg, size)){ //if the given string (with same size and font) is already cached, get it from cache
            texture_gl_legacy* Texture = gFont_cache->return_cache();
            glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
            draw_texture_(Texture, x, y);
            glColor4f(1, 1, 1, 1);
        }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
            SDL_Color color_white = {255, 255, 255, 255};
            SDL_Surface* text = TTF_RenderUTF8_Blended(font, arg2.c_str(), color_white);
            texture_gl_legacy* Texture = create_texture_from_surface(text);
            glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
            draw_texture_(Texture, x, y);
            glColor4f(1, 1, 1, 1);
            SDL_FreeSurface(text);
            gFont_cache->cache_string(arg2, Texture, arg, size);
        }
    }
}

texture_gl_legacy* renderer_opengl_legacy::create_texture_from_surface(SDL_Surface* arg){
    glEnable(GL_TEXTURE_2D);
    GLuint TextureID = new_texture_id();
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    int Mode = GL_RGB;
    if(arg->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, arg->w, arg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, arg->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    auto temp = new texture_gl_legacy(TextureID, Mode, (GLfloat)arg->w, (GLfloat)arg->h);
    return temp;
}


//This will draw text using cached glyphs - the fastest way possible, works only with english and relatively simple fonts
//(any complex cursive fonts won't be rendered properly)
void renderer_opengl_legacy::draw_text_cached(std::string arg, std::string arg2, int x, int y, SDL_Color color , int size){
    std::string font_and_size = arg+std::to_string(size);
    std::vector<texture_gl_legacy*> tempVector = fonts_cache[font_and_size];
    if(!tempVector.empty()){
        int tempX = x;
        const char* arg3 = arg2.c_str();
        glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
        for(int j = 0; arg3[j] != 0; j++){
            texture_gl_legacy* glyph = tempVector.at(static_cast<unsigned long>(arg3[j]-32));
            draw_texture_(glyph, tempX, y);
            tempX += glyph->width;
        }
        glColor4f(1, 1, 1, 1);
    }
}

void renderer_opengl_legacy::draw_texture_(texture_gl_legacy* Texture, int x, int y){
    if(Texture != nullptr){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
        GLfloat Width = Texture->width;
        GLfloat Height = Texture->height;
      	auto X = static_cast<GLfloat>(x);
        auto Y = static_cast<GLfloat>(y);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0, 0); glVertex2f(X, Y);
            glTexCoord2f(1, 0); glVertex2f(X + Width, Y);
            glTexCoord2f(0, 1); glVertex2f(X, Y + Height);
            glTexCoord2f(1, 1); glVertex2f(X + Width, Y + Height);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

//This is will decide which of the above text rendering methods to use based on the flag that is passed, if you are unsure if your
//font will render properly just pass in 0 (the default for all non-internal engine text)
void renderer_opengl_legacy::draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int flag){
    if(flag == 1){
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }else if(flag == 0){
        draw_text_normal(arg, arg2, x, y, color_font, size);
    }else{
        for(unsigned int i = 0; i < arg2.size(); i++)
            if(arg2.at(i) > 126 || arg2.at(i) < 32){
                draw_text_normal(arg, arg2, x, y, color_font, size);
                return;
            }
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }
}

void renderer_opengl_legacy::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    glColor4f((1.0f/255)*r, (1.0f/255)*g, (1.0f/255)*b, (1.0f/255)*a);

}

GLuint renderer_opengl_legacy::new_texture_id(){
    return texture_ids++;
}

void renderer_opengl_legacy::close() {
    for(auto& it : fonts){
        if(it.second != nullptr){
            if(this->fonts[it.first] != nullptr){
                for(auto& k : fonts_cache[it.first]){
                    delete k;
                }
            }
        }
    }

    for(auto& it : textures){
        if(it.second != nullptr){
            delete textures[it.first];
        }
    }

    delete gFont_cache;
    SDL_GL_DeleteContext(gl_contex);
}

void renderer_opengl_legacy::upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces){
    if(surfaces != nullptr){
        for ( auto& it : *surfaces){
            if(it.second == nullptr && textures[it.first] != nullptr){
                delete this->textures[it.first];
                textures[it.first] = nullptr;
            }
            else if(it.second != nullptr){
                if(this->textures[it.first] != nullptr){
                    delete this->textures[it.first];
                    this->textures[it.first] = nullptr;
                }
                this->textures[it.first] = create_texture_from_surface(it.second);
            }
        }
    }
}

void renderer_opengl_legacy::upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts){
    if(fonts != nullptr){
        for ( auto& it : *fonts){
            if(it.second == nullptr){
                for(auto& k : fonts_cache[it.first]){
                    delete k;
                }
                this->fonts[it.first] = nullptr;
            }
            else if(it.second != nullptr){
                if(this->fonts[it.first] != nullptr){
                    for(auto& k : fonts_cache[it.first]){
                        delete k;
                    }
                    this->fonts[it.first] = nullptr;
                }
                this->fonts[it.first] = it.second;
                cache_font(this->fonts[it.first], it.first);
            }
        }
    }
}

//do not confuse this method with the font_cache class, they have nothing in common, this caches fonts to be used
//with ASCII strings, font_cache is a LRU cache that works with any string
void renderer_opengl_legacy::cache_font(TTF_Font* Font, std::string font_and_size){
    SDL_Color color_font = {255, 255, 255, 255};
    char temp[2];
    temp[1] = 0;
    std::vector<texture_gl_legacy*> tempVector;
    for(char j = 32; j < 127; j++){
        temp[0] = j;
        SDL_Surface* glyph = TTF_RenderUTF8_Blended(Font, temp, color_font);
        texture_gl_legacy* Texture = create_texture_from_surface(glyph);
        tempVector.push_back(Texture);
        SDL_FreeSurface(glyph);
    }
    fonts_cache[font_and_size] = tempVector;
}


void renderer_opengl_legacy::vsync_on(){
    SDL_GL_SetSwapInterval(1);
}

void renderer_opengl_legacy::vsync_off(){
    SDL_GL_SetSwapInterval(0);
}


renderer_opengl_legacy::~renderer_opengl_legacy(){
    this->close();
}
