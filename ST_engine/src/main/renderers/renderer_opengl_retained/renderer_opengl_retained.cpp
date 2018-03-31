#include "renderer_opengl_retained.hpp"

GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
 	-1.0f,  1.0f,

	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f
};

GLfloat uv_data[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
 	0.0f,  0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

GLint x_to_shader;
GLint y_to_shader;
GLint texture_to_shader;

GLfloat scale_x;
GLfloat scale_y;

int atlas_w_counter = 0;
int atlas_offset_x = 0;
int atlas_offset_y = 0;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int renderer_opengl_retained::initialize(SDL_Window* window, int v_width, int v_height){
    //initialize SDL_TTF
    if(TTF_Init() < 0){
        fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
        exit(1);
    }

    //gFont_cache = new font_cache_opengl(100);
    //initialize renderer
    this->window = window;

    SDL_GLContext gl_contex = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(this->window, gl_contex);
    glewInit();
	shader_id = LoadShaders("v_shader.glsl", "f_shader.glsl");
	//shader_rect_id = LoadShaders("v_shader.glsl", "f_shader_rect.glsl");

	scale_x = 1.0f/960.0f;
	scale_y = 1.0f/540.0f;

	glGenVertexArrays(1, &VAID);
	glBindVertexArray(VAID);
	GLint loc = glGetUniformLocation(shader_id, "scale_x");
	if(loc != -1){
		glUniform1f(loc, (GLfloat)(1/960.0f));
	}
	GLint loc2 = glGetUniformLocation(shader_id, "scale_y");
	if(loc2 != -1){
		glUniform1f(loc2, (GLfloat)(1.0f/540.0f));
	}
	x_to_shader = glGetUniformLocation(shader_id, "x");
	y_to_shader = glGetUniformLocation(shader_id, "y");
	texture_to_shader = glGetUniformLocation(shader_id, "TexMap");

	int MaxTextureUnits = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureUnits);
	printf("Available Texture Units: %d\n", MaxTextureUnits);
	/*
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
*/
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

    float targetAspectRatio = virtual_width/(float)virtual_height;

    // figure out the largest area that fits in this resolution at the desired aspect ratio
    int width = screen_width ;
    int height = (int)(width / targetAspectRatio + 0.5f);

    if (height > screen_height ){
       //It doesn't fit our height, we must switch to pillarbox then
        height = screen_height ;
        width = (int)(height * targetAspectRatio + 0.5f);
    }

    float scale_x = ((float)(screen_width)) / ((float)virtual_width);
    float scale_y = ((float)(screen_height)) / ((float)virtual_height);
    glScalef(scale_x, scale_y, 1.0f);
	glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
	render_list.resize(100);
	for(int i = 0; i < 24000; i++){
		vbo_buffer[i] = 0;
	}
        int j = 0;
	for(int i = 0; i < 24000; i++){
		uv_buffer[i] = uv_data[j];
                if(j == 5){
                    j = 0;
                }else{
                    j++;
                }
	}
	main_uv = create_vbo(uv_buffer, sizeof(uv_buffer));
	glGenTextures(1, &atlases_used);
	glTexImage2D(GL_TEXTURE_2D, atlases_used, GL_RGBA, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    return 0;
}

GLuint renderer_opengl_retained::create_vbo(GLfloat* vertices, int size){
	GLuint ID;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	return ID;
}

void renderer_opengl_retained::present(){
	glEnable(GL_TEXTURE_2D);
	while(render_list.size() > 0){
		texture* Texutre = render_list.back();
		render_list.pop_back();
	}
	glUseProgram(shader_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(texture_to_shader, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, main_vbo);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   nullptr            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, main_uv);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		nullptr                        // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, (vbo_index-12)/2); //Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisable(GL_TEXTURE_2D);
    SDL_GL_SwapWindow(window);
	tex_unit = 0;
	vbo_offset = 0;
	vbo_index = 0;
}

/*
void glNamedBufferSubData(	GLuint buffer,
 	GLintptr offset,
 	GLsizei size,
 	const void *data);
*/

void renderer_opengl_retained::draw_texture(size_t arg, int x, int y){
	texture* Texture = textures[arg];
	render_list.push_back(Texture);
	vbo_buffer[0+vbo_index] = -1.0f + x*scale_x;
	vbo_buffer[1+vbo_index] = -1.0f + y*scale_y;
	vbo_buffer[2+vbo_index] = -1.0f + scale_x*(Texture->width) + x*scale_x;
	vbo_buffer[3+vbo_index] = -1.0f + y*scale_y;
	vbo_buffer[4+vbo_index] = -1.0f + x*scale_x;
	vbo_buffer[5+vbo_index] = -1.0f + scale_y*(Texture->height) + y*scale_y;
	vbo_buffer[6+vbo_index] = -1.0f + scale_x*(Texture->width) + x*scale_x;
	vbo_buffer[7+vbo_index] = -1.0f + y*scale_y;
	vbo_buffer[8+vbo_index] = -1.0f + x*scale_x;
	vbo_buffer[9+vbo_index] = -1.0f + scale_y*(Texture->height) + y*scale_y;
	vbo_buffer[10+vbo_index] = -1.0f + scale_x*(Texture->width) + x*scale_x;
	vbo_buffer[11+vbo_index] = -1.0f + scale_y*(Texture->height) + y*scale_y;
	//glNamedBufferSubData(main_vbo, vbo_index, 12, &vbo_buffer[vbo_index]);
	vbo_index += 12;
}

void renderer_opengl_retained::draw_rectangle(int x, int y, int w, int h, SDL_Color color){
    /*SDL_Rect Rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(sdl_renderer, &Rect);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);*/
}

void renderer_opengl_retained::draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color){

}

void renderer_opengl_retained::draw_background(size_t arg){
	/*texture* Texture = textures[arg];

	if(Texture != NULL){
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, UVB);
		glVertexAttribPointer(
		    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		    2,                                // size
		    GL_FLOAT,                         // type
		    GL_FALSE,                         // normalized?
		    0,                                // stride
		    (void*)0                          // array buffer offset
		);
		// Draw
        glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Send info to shader
		glUseProgram(shader_id);
	   	glUniform1f(x_to_shader, 0);
	   	glUniform1f(y_to_shader, 0);
	   	glUniform1i(texture_to_shader, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisable(GL_TEXTURE_2D);
	}*/
    texture* Texture = textures[arg];
	render_list.push_back(Texture);
	vbo_buffer[0+vbo_index] = -1.0f;
	vbo_buffer[1+vbo_index] = -1.0f;
	vbo_buffer[2+vbo_index] = -1.0f + scale_x*(Texture->width);
	vbo_buffer[3+vbo_index] = -1.0f;
	vbo_buffer[4+vbo_index] = -1.0f;
	vbo_buffer[5+vbo_index] = -1.0f + scale_y*(1080);
	vbo_buffer[6+vbo_index] = -1.0f + scale_x*(1920);
	vbo_buffer[7+vbo_index] = -1.0f;
	vbo_buffer[8+vbo_index] = -1.0f;
	vbo_buffer[9+vbo_index] = -1.0f + scale_y*(1080);
	vbo_buffer[10+vbo_index] = -1.0f + scale_x*(1920);
	vbo_buffer[11+vbo_index] = -1.0f + scale_y*(1080);
	//glNamedBufferSubData(main_vbo, vbo_index, 12, vbo_buffer);
	vbo_index += 12;

}

void renderer_opengl_retained::draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num){
    /*texture* Texture = textures[arg];
    if(Texture != NULL){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int Width = Texture->width;
        int Height = Texture->height;
        int temp1 = Height/animation_num;
        int temp2 = Width/sprite_num;
        GLfloat minx, miny, maxx, maxy;
        GLfloat minu, maxu, minv, maxv;
        SDL_Rect dstrect = {x, y - temp1, temp2, temp1};
        SDL_Rect srcrect = {sprite*(Width/sprite_num), temp1*(animation-1), temp2, temp1};

        minx = dstrect.x;
        miny = dstrect.y;
        maxx = dstrect.x + dstrect.w;
        maxy = dstrect.y + dstrect.h;

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
    }*/
}

void renderer_opengl_retained::draw_overlay(size_t arg, int sprite, int sprite_num){
    /*texture* Texture = textures[arg];
    if(Texture != NULL){
        glEnable(GL_TEXTURE_2D);
        int animation_num = 1;
        int animation = 1;
        glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int Width = Texture->width;
        int Height = Texture->height;
        int temp1 = Height/animation_num;
        int temp2 = Width/sprite_num;
        GLfloat minx, miny, maxx, maxy;
        GLfloat minu, maxu, minv, maxv;

        SDL_Rect dstrect = {0, 0, 1920, 1080};
        SDL_Rect srcrect = {sprite*(Width/sprite_num), temp1*(animation-1), temp2, temp1};

        minx = dstrect.x;
        miny = dstrect.y;
        maxx = dstrect.x + dstrect.w;
        maxy = dstrect.y + dstrect.h;

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
    }*/
}

void renderer_opengl_retained::clear_screen(){
    glClearColor(0,0,0,1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_list.clear();
	render_list.resize(200);
}

//Text fallback for non-english text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
void renderer_opengl_retained::draw_text_normal(std::string arg, std::string arg2, int x, int y, SDL_Color color, int size){
    /*std::string font_and_size = arg+std::to_string(size);
    TTF_Font* font = fonts[font_and_size];
    if(font != NULL){
        if(gFont_cache->str_is_cached(arg2, arg, size)){ //if the given string (with same size and font) is already cached, get it from cache
            texture* Texture = gFont_cache->return_cache();
            glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
            draw_texture_(Texture, x, y);
            glColor4f(1, 1, 1, 1);
        }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
            SDL_Color color_white = {255, 255, 255, 255};
            SDL_Surface* text = TTF_RenderUTF8_Blended(font, arg2.c_str(), color_white);
            texture* Texture = create_texture_from_surface(text);
            glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
            draw_texture_(Texture, x, y);
            glColor4f(1, 1, 1, 1);
            SDL_FreeSurface(text);
            gFont_cache->cache_string(arg2, Texture, arg, size);
        }
    }*/
}

texture* renderer_opengl_retained::add_to_atlas(SDL_Surface* arg){
    glEnable(GL_TEXTURE_2D);
    //GLuint TextureID = new_texture_id();

    glBindTexture(GL_TEXTURE_2D, atlases_used);
    int Mode = GL_RGB;
    if(arg->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    //glTextureSubImage2D(atlases_used , 0, atlas_offset_x, 0, arg->w, arg->h, GL_RGBA, GL_UNSIGNED_BYTE, arg->pixels);
	atlas_offset_x += arg->w;
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint texture_vbo = 0;
    texture* temp = new texture(atlases_used, atlas_offset_x, atlas_offset_y, arg->w, arg->h);

	if(atlas_offset_x > 3800){
		atlases_used++;
	}

    return temp;
}


//This will draw text using cached glyphs - the fastest way possible, works only with english and relatively simple fonts
//(any complex cursive fonts won't be rendered properly)
void renderer_opengl_retained::draw_text_cached(std::string arg, std::string arg2, int x, int y, SDL_Color color , int size){
    /*std::string font_and_size = arg+std::to_string(size);
    std::vector<texture*> tempVector = fonts_cache[font_and_size];
    if(tempVector.size() != 0){
        int tempX = x;
        const char* arg3 = arg2.c_str();
        //glColor4f((1.0f/255)*color.r, (1.0f/255)*color.g, (1.0f/255)*color.b, (1.0f/255)*color.a);
        for(int j = 0; arg3[j] != 0; j++){
            texture* glyph = tempVector.at(arg3[j]-32);
            draw_texture_(glyph, tempX, y);
            tempX += glyph->width;
        }
        //glColor4f(1, 1, 1, 1);
    }*/
}

void renderer_opengl_retained::draw_texture_(texture* Texture, int x, int y){
	/*if(Texture != NULL){
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Texture->VBO);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, UVB);
		glVertexAttribPointer(
		    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		    2,                                // size
		    GL_FLOAT,                         // type
		    GL_FALSE,                         // normalized?
		    0,                                // stride
		    (void*)0                          // array buffer offset
		);

        glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glUseProgram(shader_id);
		//Send info to shader
	   	glUniform1f(x_to_shader, (GLfloat)x);
	   	glUniform1f(y_to_shader, (1080 -(GLfloat)y));
	   	glUniform1i(texture_to_shader, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisable(GL_TEXTURE_2D);
	}*/
}

//This is will decide which of the above text rendering methods to use based on the flag that is passed, if you are unsure if your
//font will render properly just pass in 0 (the default for all non-internal engine text)
void renderer_opengl_retained::draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int flag){
    /*if(flag == 1){
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }else if(flag == 0){
        draw_text_normal(arg, arg2, x, y, color_font, size);
    }else{
        for(unsigned int i = 0; i < arg2.size(); i++)
            if(arg2.at(i) > 127 || arg2.at(i) < 32){
                draw_text_normal(arg, arg2, x, y, color_font, size);
                return;
            }
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }*/
}

void renderer_opengl_retained::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    glColor4f((1.0f/255)*r, (1.0f/255)*g, (1.0f/255)*b, (1.0f/255)*a);
}

GLuint renderer_opengl_retained::new_texture_id(){
    return id++;
}

void renderer_opengl_retained::build_atlas(){
	//for(int i = 0; i < atlases_used; i++){
		//atlas_array[i]
	//}
	atlases_used = 0;
	atlases_used++;
}

void renderer_opengl_retained::upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces){
	vbo_index = 0;
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
                this->textures[it.first] = add_to_atlas(it.second);
            }
        }
    }
	main_vbo = create_vbo(vbo_buffer, sizeof(vbo_buffer));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, main_vbo);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, main_uv);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		nullptr                        // array buffer offset
	);
}

void renderer_opengl_retained::upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts){
    /*if(fonts != NULL)
    for ( auto it = fonts->begin(); it != fonts->end(); ++it ){
        if(it->second != NULL){
            this->fonts[it->first] = it->second;
            cache_font(this->fonts[it->first], it->first);
        }
    }*/
}

//do not confuse this method with the font_cache class, they have nothing in common, this caches fonts to be used
//with ASCII strings, font_cache is a LRU cache that works with any string
void renderer_opengl_retained::cache_font(TTF_Font* Font, std::string font_and_size){
    /*SDL_Color color_font = {255, 255, 255, 255};
    char temp[2];
    temp[1] = 0;
    std::vector<texture*> tempVector;
    for(int j = 32; j < 127; j++){
        temp[0] = j;
        SDL_Surface* glyph = TTF_RenderUTF8_Blended(Font, temp, color_font);
        texture* Texture = create_texture_from_surface(glyph);
        tempVector.push_back(Texture);
        SDL_FreeSurface(glyph);
    }
    fonts_cache[font_and_size] = tempVector;*/
}

void renderer_opengl_retained::vsync_on(){
    SDL_GL_SetSwapInterval(1);
}

void renderer_opengl_retained::vsync_off(){
    SDL_GL_SetSwapInterval(0);
}

void renderer_opengl_retained::close(){
    glDeleteBuffersARB(1, &main_vbo);
}

renderer_opengl_retained::~renderer_opengl_retained(){
    this->close();
}
