/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef CONSOLE_LOG_HPP
#define CONSOLE_LOG_HPP

#include <defs.hpp>



namespace ST {

    /**
     * Enum representing the different log types.
     */
    enum class log_type : uint8_t {ERROR = 0x01, SUCCESS = 0x02, INFO = 0x04};

    /**
     *
     * @param a A log_type enum.
     * @param b A log_type enum.
     * @return The result of binary OR performed on <b>a</b> and <b>b</b>
     */
    inline log_type operator | (log_type a, log_type b){
        return (log_type)(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    ///Represents a log message to be displayed in the console.
    /**
     * Contains the color, text and type of the log.
     */
    class console_log {
    public:
        SDL_Color color{};
        std::string text;
        ST::log_type type;

        console_log(log_type type, const std::string &text);
    };
}


#endif
