/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef CONSOLE_LOG_HPP
#define CONSOLE_LOG_HPP


#include <cstdint>
#include <string>
#include <SDL2/SDL_pixels.h>

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
        return static_cast<log_type>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
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
