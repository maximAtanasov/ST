/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <console/console_log.hpp>

/**
 * Constructor for the <b>ST::console_log</b> object.
 * @param type The log type - this can be log_type ::ERROR, log_type::SUCCESS or log_type::INFO.
 * @param text The text the log should contain.
 */
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