/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

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