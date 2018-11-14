/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <string>
#include <ST_loaders/loaders.hpp>

/**
 * Gets the file extension from the filename.
 * @param filename The filename.
 * @return The extension. - "png", "webp", "wav", "mp3", or "ogg". Unknown when it's well, unknown.
 */
std::string ST::get_file_extension(const std::string& filename){
    if(filename.size() > 4){
        uint64_t size = filename.size() - 1;
        if(filename.at(size-3) == '.' && filename.at(size - 2) == 'p' && filename.at(size - 1) == 'n' && filename.at(size) == 'g'){
            return "png";
        }else if(filename.at(size-3) == '.' && filename.at(size - 2) == 'w' && filename.at(size - 1) == 'a' && filename.at(size) == 'v'){
            return "wav";
        }else if(filename.at(size-3) == '.' && filename.at(size - 2) == 'm' && filename.at(size - 1) == 'p' && filename.at(size) == '3'){
            return "mp3";
        }else if(filename.at(size-3) == '.' && filename.at(size - 2) == 'o' && filename.at(size - 1) == 'g' && filename.at(size) == 'g'){
            return "ogg";
        }else if(filename.at(size-3) == '.' && filename.at(size - 2) == 'b' && filename.at(size - 1) == 'i' && filename.at(size) == 'n'){
            return "bin";
        }else if(filename.size() > 5){
            if(filename.at(size - 4) == '.' && filename.at(size - 3) == 'w'
               && filename.at(size - 2) == 'e' && filename.at(size - 1) == 'b' && filename.at(size) == 'p'){
                return "webp";
            }
        }
    }
    return "unknown";
}

/**
 * Packs assets to a binary.
 * Assets must be present on disk.
 * @param binary The name of the binary that is going to be created.
 * @param args The filenames of the assets to read from.
 */
int8_t ST::pack_to_binary(const std::string& binary, std::vector<std::string> args_){
    FILE* file = fopen(binary.c_str(), "r+");
    if(file != nullptr){
        fclose(file);
        return ST::add_to_binary(binary, args_);
    }
    SDL_RWops *output = SDL_RWFromFile(binary.c_str(), "a+");

    std::vector<std::string> args;

    //ignore duplicate file names
    for (auto arg : args_) {
        if(std::find(args.begin(), args.end(), arg) == args.end()){
            args.emplace_back(arg);
        }
    }

    std::vector<std::string> surfaces_names;
    std::vector<char*> surfaces;
    std::vector<size_t> surfaces_sizes;

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
            std::string h_filename = "filename:" + trim_path(filename) + "\n";
            std::string ext = get_file_extension(filename);
            if(ext == "png" || "webp") {
                surfaces_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                surfaces.emplace_back(temp);
                surfaces_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }else if(ext == "wav") {
                chunks_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                chunks.emplace_back(temp);
                chunk_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }else if(ext == "ogg") {
                music_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                music.emplace_back(temp);
                music_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }
            input->close(input);
        }
    }

    //Package into a single file

    //write our header===

    //total number of assets
    uint64_t total_size = surfaces_names.size() + music_names.size() + chunks_names.size();
    std::string total_num = "total:" + std::to_string(total_size) + "\n";
    output->write(output, total_num.c_str(), 1, total_num.size());

    //Names and sizes for surfaces, chunks and music
    for(uint64_t i = 0; i < surfaces_sizes.size(); i++) {
        output->write(output, surfaces_names[i].c_str(), 1, surfaces_names[i].size());
        std::string size = "size:" + std::to_string(surfaces_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    for(uint64_t i = 0; i < chunk_sizes.size(); i++) {
        output->write(output, chunks_names[i].c_str(), 1, chunks_names[i].size());
        std::string size = "size:" + std::to_string(chunk_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    for(uint64_t i = 0; i < music_sizes.size(); i++) {
        output->write(output, music_names[i].c_str(), 1, music_names[i].size());
        std::string size = "size:" + std::to_string(music_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    //Write the data===

    //Write image data
    for(uint64_t i = 0; i < surfaces.size(); i++) {
        output->write(output, surfaces[i], 1, surfaces_sizes[i]);
    }

    //Write chunks data
    for(uint64_t i = 0; i < chunks.size(); i++) {
        output->write(output, chunks[i], 1, chunk_sizes[i]);
    }

    //Write music data
    for(uint64_t i = 0; i < music.size(); i++) {
        output->write(output, music[i], 1, music_sizes[i]);
    }
    output->close(output);
    return 0;
}

/**
 * Unpacks all the assets a binary contains.
 * @param path The path to the binary.
 * @return An ST:assets_named struct containing all assets in native engine format and their names, <b>nullptr</b> on error.
 */
ST::assets_named* ST::unpack_binary(const std::string& path){
    auto assets = new ST::assets_named();
    SDL_RWops *input = SDL_RWFromFile(path.c_str(), "r");
    if (input != nullptr) {
        auto buffer = static_cast<char*>(malloc((size_t) input->size(input)));
        size_t read = input->read(input, buffer, 1, (size_t) input->size(input));
        if (read > 0) {
            auto file = new std::string(buffer);
            std::vector<std::string> file_names;
            std::vector<size_t> sizes;

            //read filename
            std::string temp;
            uint64_t total_num = 0;
            uint64_t counter = 0;
            uint64_t pointer = 0;
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
            uint64_t seek = pointer;
            uint64_t i = 0;
            for (const std::string &filename : file_names) {
                std::string ext = get_file_extension(filename);
                if(ext == "png") {
                    seek += sizes.at(i);
                    auto to_write = static_cast<char*>(malloc(sizes.at(i)));
                    input->read(input, to_write, 1, sizes.at(i));
                    SDL_RWops* output = SDL_RWFromMem(to_write, static_cast<int>(sizes.at(i)));
                    SDL_Surface* temp_surface = IMG_LoadPNG_RW(output);
                    if(temp_surface != nullptr) {
                        assets->surfaces[filename] = temp_surface;
                    }
                    SDL_RWclose(output);
                    free(to_write);
                    input->seek(input, seek, RW_SEEK_SET);
                }else if(ext == "webp") {
                    seek += sizes.at(i);
                    auto to_write = static_cast<char*>(malloc(sizes.at(i)));
                    input->read(input, to_write, 1, sizes.at(i));
                    SDL_RWops* output = SDL_RWFromMem(to_write, static_cast<int>(sizes.at(i)));
                    SDL_Surface* temp_surface = IMG_LoadWEBP_RW(output);
                    if(temp_surface != nullptr) {
                        assets->surfaces[filename] = temp_surface;
                    }
                    SDL_RWclose(output);
                    free(to_write);
                    input->seek(input, seek, RW_SEEK_SET);
                }else if(ext == "wav"){
                    seek += sizes.at(i);
                    auto to_write = static_cast<char*>(malloc(sizes.at(i)));
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
                    auto to_write = static_cast<char*>(malloc(sizes.at(i)));
                    input->read(input, to_write, 1, sizes.at(i));
                    SDL_RWops* output = SDL_RWFromMem(to_write, static_cast<int>(sizes.at(i)));
                    Mix_Music* temp_music = Mix_LoadMUSType_RW(output, MUS_OGG, 1);
                    if(temp_music != nullptr) {
                        assets->music[filename] = temp_music;
                    }else{
                        free(to_write);
                    }
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

/**
 * Unpacks the contents of a binary to disk.
 * @param path The path to the binary.
 * @return 0 on Success, -1 on Failure.
 */
int8_t ST::unpack_binary_to_disk(const std::string& path){
    SDL_RWops *input = SDL_RWFromFile(path.c_str(), "r");
    if (input != nullptr) {
        auto buffer = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
        size_t read = input->read(input, buffer, 1, (size_t) input->size(input));
        if (read > 0) {
            auto file = new std::string(buffer);
            std::vector<std::string> file_names;
            std::vector<size_t> sizes;
            //read filename
            std::string temp;
            uint64_t total_num = 0;
            uint64_t counter = 0;
            uint64_t pointer = 0;
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
            uint64_t seek = pointer;
            uint64_t i = 0;
            for (const std::string &filename : file_names) {
                std::string ext = get_file_extension(filename);
                if(ext == "png" || ext == "webp" || ext == "wav" || ext == "ogg") {
                    SDL_RWops *output = SDL_RWFromFile(filename.c_str(), "w");
                    seek += sizes.at(i);
                    auto to_write = static_cast<char*>(malloc(sizes.at(i)));
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


/**
 * 
 * @param binary_name The name of the already existing binary.
 * @param args_ The new files to add to it.
 * @return -1 if the existing binary is not found. -2 if file names are clashing with the ones in the existing library. 0 on success.
 */
int8_t ST::add_to_binary(const std::string &binary_name, std::vector<std::string> args_){

    SDL_RWops *output = SDL_RWFromFile(binary_name.c_str(), "r+");

    std::vector<std::string> args;

    //ignore duplicate file names
    for (auto arg : args_) {
        if(std::find(args.begin(), args.end(), arg) == args.end()){
            args.emplace_back(arg);
        }
    }

    //Read in the old header first
    std::vector<std::string> old_header_file_names;
    std::vector<size_t> old_header_sizes;
    uint64_t old_header_total_num = 0;
    uint64_t seek = 0;

    if (output != nullptr) {
        auto buffer = static_cast<char*>(malloc(static_cast<size_t>(output->size(output))));
        size_t read = output->read(output, buffer, 1, (size_t) output->size(output));
        if (read > 0) {
            auto file = new std::string(buffer);
            //read filename
            std::string temp;
            uint64_t counter = 0;
            uint64_t pointer = 0;
            for (char i : *file) {
                pointer++;
                temp += i;
                if (i == '\n') {
                    if (temp.find("filename:") != std::string::npos) {
                        temp.pop_back();
                        replace_string(temp, "filename:", "");
                        old_header_file_names.emplace_back(temp);
                        temp.clear();
                    } else if (temp.find("total:") != std::string::npos) {
                        temp.pop_back();
                        replace_string(temp, "total:", "");
                        std::stringstream s_stream(temp);
                        s_stream >> old_header_total_num;
                        temp.clear();
                    } else if (temp.find("size:") != std::string::npos) {
                        temp.pop_back();
                        replace_string(temp, "size:", "");
                        std::stringstream s_stream(temp);
                        int size;
                        s_stream >> size;
                        old_header_sizes.emplace_back(size);
                        temp.clear();
                        counter++;
                    }
                    if (counter == old_header_total_num) {
                        break;
                    }
                }
            }
            free(buffer);

            output->seek(output, pointer, RW_SEEK_SET);
            seek = pointer;
        }
    }else{
        //If the file could not be read
        printf("HAHAA");
        return -1;
    }

    //Check for duplicate file names inside the existing library, stop execution if any are found
    for(const std::string& name : args){
        for(const std::string& existing_name : old_header_file_names){
            if(trim_path(name) == existing_name){
                output->close(output);
                return -2;
            }
        }
    }


    //Save the old binary data
    size_t old_binary_data_size = output->size(output) - seek;
    auto buffer = static_cast<char*>(malloc(static_cast<size_t>(old_binary_data_size)));
    output->read(output, buffer, 1, old_binary_data_size);

    //Read in new files

    std::vector<std::string> surfaces_names;
    std::vector<char*> surfaces;
    std::vector<size_t> surfaces_sizes;

    std::vector<std::string> chunks_names;
    std::vector<char*> chunks;
    std::vector<size_t> chunk_sizes;

    std::vector<std::string> music_names;
    std::vector<char*> music;
    std::vector<size_t> music_sizes;

    for (const std::string& filename : args) {
        SDL_RWops *input = SDL_RWFromFile(filename.c_str(), "r+b");
        if (input != nullptr) {
            std::string h_filename = "filename:" + trim_path(filename) + "\n";
            std::string ext = get_file_extension(filename);
            if(ext == "png" || "webp") {
                surfaces_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                surfaces.emplace_back(temp);
                surfaces_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }else if(ext == "wav") {
                chunks_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                chunks.emplace_back(temp);
                chunk_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }else if(ext == "ogg") {
                music_names.emplace_back(h_filename);
                auto temp = static_cast<char*>(malloc(static_cast<size_t>(input->size(input))));
                input->read(input, temp, 1, static_cast<size_t>(input->size(input)));
                music.emplace_back(temp);
                music_sizes.emplace_back(static_cast<size_t>(input->size(input)));
            }
            input->close(input);
        }
    }

    //Package into a single file

    //write our header==================================================

    //Delete old binary and create a new one
    output->close(output);
    output = SDL_RWFromFile(binary_name.c_str(), "w+");

    //Write the new total
    uint64_t total_size = surfaces_names.size() + music_names.size() + chunks_names.size();
    std::string total_num = "total:" + std::to_string(total_size+old_header_total_num) + "\n";
    output->write(output, total_num.c_str(), 1, total_num.size());

    //The old data gets written first
    for(uint64_t i = 0; i < old_header_file_names.size(); i++) {
        std::string h_filename = "filename:" + old_header_file_names[i] + "\n";
        output->write(output, h_filename.c_str(), 1, h_filename.size());
        std::string size = "size:" + std::to_string(old_header_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    //And now the new files =============================================

    //Names and sizes for surfaces, chunks and music
    for(uint64_t i = 0; i < surfaces_sizes.size(); i++) {
        output->write(output, surfaces_names[i].c_str(), 1, surfaces_names[i].size());
        std::string size = "size:" + std::to_string(surfaces_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    for(uint64_t i = 0; i < chunk_sizes.size(); i++) {
        output->write(output, chunks_names[i].c_str(), 1, chunks_names[i].size());
        std::string size = "size:" + std::to_string(chunk_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    for(uint64_t i = 0; i < music_sizes.size(); i++) {
        output->write(output, music_names[i].c_str(), 1, music_names[i].size());
        std::string size = "size:" + std::to_string(music_sizes[i]) + "\n";
        output->write(output, size.c_str(), 1, size.size());
    }

    //Write the data===

    //Write the old binary data
    output->write(output, buffer, 1, old_binary_data_size);

    // Write the new binary data

    //Write image data
    for(uint64_t i = 0; i < surfaces.size(); i++) {
        output->write(output, surfaces[i], 1, surfaces_sizes[i]);
    }

    //Write chunks data
    for(uint64_t i = 0; i < chunks.size(); i++) {
        output->write(output, chunks[i], 1, chunk_sizes[i]);
    }

    //Write music data
    for(uint64_t i = 0; i < music.size(); i++) {
        output->write(output, music[i], 1, music_sizes[i]);
    }
    output->close(output);
    return 0;
}