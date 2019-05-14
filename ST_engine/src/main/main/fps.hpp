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
/**
 * Provides average frame-rates over 1 second.
 * We need this class, because the framerate is otherwise constantly fluctuating.
 */
class fps {

    private:
        int counter = 0;
        double old_time = 0;
        double new_time = 0;
        double value = 0;
        double average[60]{};
        double get_average();
    public:
        fps();
        void update(double time, double frame_time);
        double get_value() const;
};

//INLINED METHODS

/**
 * @return The current average framerate value.
 */
inline double fps::get_value() const{
    return value;
}

#endif
