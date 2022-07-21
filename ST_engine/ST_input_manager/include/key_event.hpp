/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_KEY_EVENT_HPP
#define ST_KEY_EVENT_HPP

#include <cstdint>

namespace ST {

    enum class key_event : uint8_t {
        PRESS = 0,
        HOLD = 1,
        RELEASE = 2
    };
}

#endif //ST_KEY_EVENT_HPP
