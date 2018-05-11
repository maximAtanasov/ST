/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef FPS_DEF
#define FPS_DEF

///An fps counter
/**
 * Provides average framerates over 1 second.
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
        double get_value();
};

//INLINED METHODS

/**
 * @return The current average framerate value.
 */
inline double fps::get_value() {
    return value;
}

#endif
