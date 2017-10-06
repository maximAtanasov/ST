#include "renderer.hpp"
#include "level_manager.hpp"
#include "assets_manager.hpp"

bool collisionsShown = false;

int D_RENDERTEXT_SMOOTH_VAL = 1;
int D_RENDERTEXT_FAST_VAL = 0;
int* D_RENDERTEXT_SMOOTH = &D_RENDERTEXT_SMOOTH_VAL;
int* D_RENDERTEXT_FAST = &D_RENDERTEXT_FAST_VAL;

Camera camera = {0, 0, -1, 3000, 0, 3000};

renderer::renderer(){}

int renderer::initialize(){

    //initialize SDL_TTF
    if(TTF_Init() < 0){
        std::cerr << "Failed to initialize SDL_TTF: " << TTF_GetError() << "\n";
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG) < 0){
        std::cerr << "Failed to initialize SDL_IMG: " << IMG_GetError() << "\n";
        exit(1);
    }

    //initialize renderer
    vsync = 1;
    sdl_renderer = SDL_CreateRenderer( gDisplay_manager.get_window() , -1, SDL_RENDERER_ACCELERATED ); //Hardware acceleration
    SDL_RenderSetLogicalSize(sdl_renderer, 1920, 1080); //Internal rendering canvas - don't touch this like ever
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ); //Linear texture filtering
    set_draw_color(0, 0, 0, 255);
    return 0;
}



void renderer::draw_texture(int arg, int x, int y){
    SDL_Texture* temp = gAssets_manager.get_assets()->textures[arg];
    int texW, texH;
    SDL_QueryTexture(temp, NULL, NULL, &texW, &texH);
    SDL_Rect srcRect = {x, y - texH, texW, texH};
	SDL_RenderCopy(sdl_renderer, temp, NULL, &srcRect);
}

void renderer::draw_rectangle(int x, int y, int w, int h, SDL_Color color){
    SDL_Rect Rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(sdl_renderer, &Rect);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

void renderer::draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color){
    SDL_Rect Rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(sdl_renderer, &Rect);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

void renderer::draw_background(int arg){
    SDL_Texture* temp = gAssets_manager.get_assets()->textures[arg];
    SDL_RenderCopy(sdl_renderer, temp, NULL, NULL);
}

void renderer::draw_sprite(int arg, int x, int y, int sprite, int animation, int animationNum, int spriteNum){
    SDL_Texture* temp = gAssets_manager.get_assets()->textures[arg];
    int texW, texH;
    SDL_QueryTexture(temp, NULL, NULL, &texW, &texH);
    int temp1 = texH/animationNum;
    int temp2 = texW/spriteNum;
    SDL_Rect srcRect = {x, y - temp1, temp2, temp1};
    SDL_Rect dstRect = {sprite*(texW/spriteNum), temp1*(animation-1), temp2, temp1};
	SDL_RenderCopy(sdl_renderer, temp, &dstRect, &srcRect);
}

//Text fallback for non-english text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
void renderer::draw_text_normal(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size){
    std::string font_and_size = arg+std::to_string(size);
    TTF_Font* font = gAssets_manager.get_assets()->fonts_assets.fonts[font_and_size];
    int texW, texH;
    if(gFont_cache.StrIsCached(arg2, arg, size)){ //if the given string (with same size and font) is already cached, get it from cache
        SDL_Texture* texture = gFont_cache.ReturnCache();
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect Rect = {x, y, texW, texH};
        SDL_RenderCopy(sdl_renderer, texture, NULL, &Rect);
    }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
        SDL_Surface* text = TTF_RenderUTF8_Blended(font, arg2.c_str(), color_font);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, text);
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect Rect = {x, y, texW, texH};
        SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
        SDL_RenderCopy(sdl_renderer, texture, NULL, &Rect);
        SDL_FreeSurface(text);  
        gFont_cache.cacheString(arg2, texture, arg, size);
    }
}


//This will draw text using cached glyphs - the fastest way possible, works only with english and relatively simple fonts
//(any complex cursive fonts won't be rendered properly)
void renderer::draw_text_cached(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size){
    std::string font_and_size = arg+std::to_string(size);
    std::vector<SDL_Texture*> tempVector = gAssets_manager.get_assets()->fonts_assets.cache[font_and_size];
    int texW, texH;
    int tempX = x;
    const char* arg3 = arg2.c_str();
    for(int j = 0; arg3[j] != 0; j++){
        SDL_Texture* texture = tempVector.at(arg3[j]-32);
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect Rect = {tempX, y, texW, texH};
        SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
        SDL_RenderCopy(sdl_renderer, texture, NULL, &Rect);
        tempX += texW;
    }
}

//This is will decide which of the above text rendering methods to use based on the flag that is passed, if you are unsure if your
//font will render properly just pass in D_RENDERTEXT_SMOOTH (the default for all non-internal engine text)
void renderer::draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int* flag){
    if(flag == D_RENDERTEXT_FAST){
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }else if(flag == D_RENDERTEXT_SMOOTH){
        draw_text_normal(arg, arg2, x, y, color_font, size);
    }else{
        for(unsigned int i = 0; i < arg2.size(); i++)
            if(arg2.at(i) > 127 || arg2.at(i) < 32){
                draw_text_normal(arg, arg2, x, y, color_font, size);
                return;
            }
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }
}

//returns a pointer to the SDL_Renderer. Didn't want to do this, but it is requered in some other subsystems
//(i.e. for caching fonts, as textures need to be put in VRAM)
SDL_Renderer* renderer::get_renderer(void){
    return sdl_renderer;
}

void renderer::set_draw_color(int r, int g, int b, int a){
    SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
}

int renderer::get_vsync(void){
    return vsync;
}

void renderer::vsync_on(void){
    SDL_GL_SetSwapInterval(1);
    vsync = 1;
}

void renderer::vsync_off(void){
    SDL_GL_SetSwapInterval(0);
    vsync = 0;
}

void renderer::close(void){
    IMG_Quit();
    TTF_Quit();
}

renderer::~renderer(){

}
