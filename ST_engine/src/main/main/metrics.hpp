/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_METRICS_H
#define ST_METRICS_H
namespace ST {
    struct metrics {
        double physics_time;
        double game_logic_time;
        double render_time;
        double frame_time;

        void reset_accumulators() {
            physics_time = 0;
            game_logic_time = 0;
        }
    };
}


#endif //ST_METRICS_H
