/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef SLAVICTALES_STRING_UTIL_HPP
#define SLAVICTALES_STRING_UTIL_HPP

#include <string>

namespace ST {

    bool replace_string(std::string &str, const std::string &from, const std::string &to);

    std::string trim_path(std::string path);

    uint16_t hash_string(const std::string &value);

}
#endif //SLAVICTALES_STRING_UTIL_HPP
