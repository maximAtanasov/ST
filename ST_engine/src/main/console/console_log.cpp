#include <console/console_log.hpp>


ST::console_log::console_log(log_type type, const std::string& text) {
    if(type == log_type ::ERROR){
        color = {255, 0 ,0 , 255};
    }else if(type == log_type::SUCCESS){
        color = {50, 255, 10, 255};
    }else if(type == log_type::INFO){
        color = {10, 50, 255, 255};
    }
    this->type = type;
    this->text = text;
}