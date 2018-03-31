#include "../../include/ST_loaders/loaders.hpp"

bool ST::replace_string(std::string& str, const std::string& from, const std::string& to){
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos){
        return false;
    }
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string ST::get_file_extension(const std::string& filename){
    if(filename.size() > 4){
        Uint64 size = filename.size() - 1;
        if(filename.at(size - 2) == 'p' && filename.at(size - 1) == 'n' && filename.at(size) == 'g'){
            return "png";
        }else if(filename.at(size - 2) == 'w' && filename.at(size - 1) == 'a' && filename.at(size) == 'v'){
            return "wav";
        }else if(filename.at(size - 2) == 'm' && filename.at(size - 1) == 'p' && filename.at(size) == '3'){
            return "mp3";
        }else if(filename.at(size - 2) == 'o' && filename.at(size - 1) == 'g' && filename.at(size) == 'g'){
            return "ogg";
        }
    }
    return "unknown";
}

void ST::pack_to_binary(const std::string& binary, std::vector<std::string> args){
    SDL_RWops *output = SDL_RWFromFile(binary.c_str(), "a+");

    std::vector<std::string> surfaces_names;
    std::vector<SDL_Surface*> surfaces;

    std::vector<std::string> chunks_names;
    std::vector<char*> chunks;
    std::vector<size_t> chunk_sizes;

    std::vector<std::string> music_names;
    std::vector<char*> music;
    std::vector<size_t> music_sizes;

    //Read files
    for (const std::string& filename : args) {
        SDL_RWops *input = SDL_RWFromFile(filename.c_str(), "r+b");
        if (input != nullptr) {
            std::string h_filename = "filename:" + filename + "\n";
            std::string ext = get_file_extension(filename);
            if(ext == "png") {
                surfaces_names.emplace_back(h_filename);
                SDL_Surface* temp = IMG_Load_RW(input, 0);
                surfaces.emplace_back(temp);
            }else if(ext == "wav") {
                chunks_names.emplace_back(h_filename);
                auto temp = (char*)malloc((size_t)input->size(input));
                input->read(input, temp, 1, (size_t)input->size(input));
                chunks.emplace_back(temp);
                chunk_sizes.emplace_back((size_t)input->size(input));
            }else if(ext == "ogg") {
                music_names.emplace_back(h_filename);
                auto temp = (char*)malloc((size_t)input->size(input));
                input->read(input, temp, 1, (size_t)input->size(input));
                music.emplace_back(temp);
                music_sizes.emplace_back((size_t)input->size(input));
            }
            input->close(input);
        }
    }

    //Package into a single file

    //write our header===

    //total number of assets
    Uint64 total_size = surfaces_names.size() + music_names.size() + chunks_names.size();
    std::string total_num = "total:" + std::to_string(total_size) + "\n";
    output->write(output, total_num.c_str(), 1, total_num.size());

    //their names
    for(const std::string& i : surfaces_names) {
        output->write(output, i.c_str(), 1, i.size());
        std::string size = "size: null(size not needed)\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    //Names and sizes for chunks and music
    for(Uint64 i = 0; i < chunk_sizes.size(); i++) {
        output->write(output, chunks_names[i].c_str(), 1, chunks_names[i].size());
        std::string size = "size:" + std::to_string(chunk_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    for(Uint64 i = 0; i < music_sizes.size(); i++) {
        output->write(output, music_names[i].c_str(), 1, music_names[i].size());
        std::string size = "size:" + std::to_string(music_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    //Write image data
    for (auto surface : surfaces) {
        IMG_SavePNG_RW(surface, output, 0);
        SDL_FreeSurface(surface);
    }

    //Write chunks data
    for(Uint64 i = 0; i < chunks.size(); i++) {
        output->write(output, chunks[i], 1, chunk_sizes[i]);
    }

    //Write music data
    for(Uint64 i = 0; i < music.size(); i++) {
        output->write(output, music[i], 1, music_sizes[i]);
    }
    output->close(output);
}

ST::assets_named* ST::unpack_binary(const std::string& path){
    auto assets = new ST::assets_named();
    SDL_RWops *input = SDL_RWFromFile(path.c_str(), "r");
    if (input != nullptr) {
        auto buffer = (char*)malloc((size_t) input->size(input));
        size_t read = input->read(input, buffer, 1, (size_t) input->size(input));
        if (read > 0) {
            std::string* file = new std::string(buffer);
            std::vector<std::string> file_names;
            std::vector<size_t> sizes;

            //read filename
            std::string temp;
            Uint64 total_num = 0;
            Uint64 counter = 0;
            Uint64 pointer = 0;
            for(char i : *file){
                pointer++;
                temp += i;
                if (i == '\n'){
                    if (temp.find("filename:") != std::string::npos){
                        temp.pop_back();
                        replace_string(temp, "filename:", "");
                        file_names.emplace_back(temp);
                        temp.clear();
                    } else if (temp.find("total:") != std::string::npos){
                        temp.pop_back();
                        replace_string(temp, "total:", "");
                        std::stringstream s_stream(temp);
                        s_stream >> total_num;
                        temp.clear();
                    } else if (temp.find("size:") != std::string::npos){
                        temp.pop_back();
                        replace_string(temp, "size:", "");
                        std::stringstream s_stream(temp);
                        int size;
                        s_stream >> size;
                        sizes.emplace_back(size);
                        temp.clear();
                        counter++;
                    }
                    if (counter == total_num){
                        break;
                    }
                }
            }
            free(buffer);
            input->seek(input, pointer, RW_SEEK_SET);
            Uint64 seek = pointer;
            Uint64 i = 0;
            for (const std::string &filename : file_names) {
                std::string ext = get_file_extension(filename);
                if(ext == "png") {
                    SDL_Surface *temp_surface = IMG_Load_RW(input, 0);
                    if (temp_surface != nullptr) {
                        assets->surfaces[filename] = temp_surface;
                    }
                    seek += SDL_RWtell(input) - seek + 16;
                    input->seek(input, seek, RW_SEEK_SET);
                }else if(ext == "wav"){
                    seek += sizes.at(i);
                    auto to_write = (char*)malloc(sizes.at(i));
                    input->read(input, to_write, 1, sizes.at(i));
                    SDL_RWops* output = SDL_RWFromMem(to_write, static_cast<int>(sizes.at(i)));
                    Mix_Chunk* temp_chunk = Mix_LoadWAV_RW(output, 1);
                    if(temp_chunk != nullptr) {
                        assets->chunks[filename] = temp_chunk;
                    }
                    free(to_write);
                    input->seek(input, seek, RW_SEEK_SET);
                }else if(ext == "ogg"){
                    seek += sizes.at(i);
                    auto to_write = (char*)malloc(sizes.at(i));
                    input->read(input, to_write, 1, sizes.at(i));
                    SDL_RWops* output = SDL_RWFromMem(to_write, static_cast<int>(sizes.at(i)));
                    Mix_Music* temp_music = Mix_LoadMUS_RW(output, 1);
                    if(temp_music != nullptr) {
                        assets->music[filename] = temp_music;
                    }
                    free(to_write);
                    input->seek(input, seek, RW_SEEK_SET);
                }
                i++;
            }
            delete file;
        }
        input->close(input);
    } else {
        return nullptr;
    }
    return assets;
}

int ST::unpack_binary_to_disk(const std::string& path){
    SDL_RWops *input = SDL_RWFromFile(path.c_str(), "r");
    if (input != nullptr) {
        auto buffer = (char*)malloc((size_t) input->size(input));
        size_t read = input->read(input, buffer, 1, (size_t) input->size(input));
        if (read > 0) {
            std::string* file = new std::string(buffer);
            std::vector<std::string> file_names;
            std::vector<size_t> sizes;
            //read filename
            std::string temp;
            Uint64 total_num = 0;
            Uint64 counter = 0;
            Uint64 pointer = 0;
            for(char i : *file) {
                pointer++;
                temp += i;
                if (i == '\n') {
                    if (temp.find("filename:") != std::string::npos){
                        temp.pop_back();
                        replace_string(temp, "filename:", "");
                        file_names.emplace_back(temp);
                        temp.clear();
                    } else if (temp.find("total:") != std::string::npos){
                        temp.pop_back();
                        replace_string(temp, "total:", "");
                        std::stringstream s_stream(temp);
                        s_stream >> total_num;
                        temp.clear();
                    } else if (temp.find("size:") != std::string::npos) {
                        temp.pop_back();
                        replace_string(temp, "size:", "");
                        std::stringstream s_stream(temp);
                        int size;
                        s_stream >> size;
                        sizes.emplace_back(size);
                        temp.clear();
                        counter++;
                    }
                    if (counter == total_num) {
                        break;
                    }
                }
            }
            free(buffer);

            input->seek(input, pointer, RW_SEEK_SET);
            Uint64 seek = pointer;
            Uint64 i = 0;
            for (const std::string &filename : file_names) {
                std::string ext = get_file_extension(filename);
                if(ext == "png") {
                    SDL_Surface *temp_surface = IMG_Load_RW(input, 0);
                    if (temp_surface != nullptr) {
                        SDL_RWops *output = SDL_RWFromFile(filename.c_str(), "w");
                        IMG_SavePNG_RW(temp_surface, output, 0);
                        seek += output->size(output);
                        input->seek(input, seek, RW_SEEK_SET);
                        output->close(output);
                        SDL_FreeSurface(temp_surface);
                    }else{
                        return -1;
                    }
                }else if(ext == "wav"){
                    auto current_seek = static_cast<size_t>(SDL_RWtell(input));
                    Mix_Chunk* temp_chunk = Mix_LoadWAV_RW(input, 0);
                    auto after_seek = static_cast<size_t>(SDL_RWtell(input));
                    if (temp_chunk != nullptr) {
                        SDL_RWops *output = SDL_RWFromFile(filename.c_str(), "w");
                        seek += after_seek-current_seek;
                        auto to_write = (char*)malloc(after_seek-current_seek);
                        input->seek(input, current_seek, RW_SEEK_SET);
                        input->read(input, to_write, 1, after_seek-current_seek);
                        output->write(output, to_write, 1, after_seek- current_seek);
                        output->close(output);
                        free(to_write);
                        Mix_FreeChunk(temp_chunk);
                    }else{
                        return -1;
                    }
                }else if(ext == "ogg"){
                    SDL_RWops *output = SDL_RWFromFile(filename.c_str(), "w");
                    seek += sizes.at(i);
                    auto to_write = (char*)malloc(sizes.at(i));
                    input->read(input, to_write, 1, sizes.at(i));
                    output->write(output, to_write, 1, sizes.at(i));
                    output->close(output);
                    free(to_write);
                    input->seek(input, seek, RW_SEEK_SET);
                }else{
                    return -1;
                }
                i++;
            }
            delete file;
        }
        input->close(input);
    } else{
        return -1;
    }
    return 0;
}