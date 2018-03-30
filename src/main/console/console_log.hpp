
#ifndef CONSOLE_LOG_HPP
#define CONSOLE_LOG_HPP


#include "../defs.hpp"

enum class log_type : Uint8 {ERROR = 0x01, SUCCESS = 0x02, INFO = 0x04};

inline log_type operator | (log_type a, log_type b){
    return (log_type)(static_cast<Uint8>(a) | static_cast<Uint8>(b));
}

class console_log{
public:
    SDL_Color color{};
    std::string text;
    log_type type;

    console_log(log_type type, const std::string& text);
};


#endif
