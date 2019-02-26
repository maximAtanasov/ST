/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <ST_util/string_util.hpp>
#include <vector>

namespace ST {

    bool replace_string(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos) {
            return false;
        }
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


    static std::vector<size_t> string_hashes;

    static std::hash<std::string> hash_f;

    uint16_t hash_string(const std::string &value) {
        size_t hashed_value = hash_f(value);

        //Now just a simple linear search
        uint16_t i = 0;
        for (i = 0; i < string_hashes.size(); i++) {
            if (string_hashes.at(i) == hashed_value) {
                return i;
            }
        }
        string_hashes.emplace_back(hashed_value);
        return i;
    }
}