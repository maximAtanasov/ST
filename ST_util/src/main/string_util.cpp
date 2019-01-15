/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <ST_util/string_util.hpp>

bool replace_string(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string trim_path(std::string path){
    for(uint32_t i = 0; i < path.size(); i++) {
        if (path.at(i) == '/') {
            path.erase(0, i+1);
        }
    }
    return path;
}