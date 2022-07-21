/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef FPS_DEF
#define FPS_DEF

///An fps counter
#include <cstdint>
#include <algorithm>

/**
 * Provides average frame-rates over ~1 second.
 * We need this class, because the framerate is otherwise constantly fluctuating.
 */
class fps {

    private:
        double old_time = 0;
        double new_time = 0;
        float value = 0;
        float average[16]{};
        float get_average();
        uint8_t counter = 0;
public:
        void update(double time, double frame_time);
        [[nodiscard]] float get_value() const;
};

#endif //FPS_DEF
