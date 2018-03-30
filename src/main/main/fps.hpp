#ifndef FPS_DEF
#define FPS_DEF

//An fps counter

class fps {
private:
    int counter = 0;
    double old_time = 0;
    double new_time = 0;
    double value = 0;
    double average[60]{};
    double get_average();
public:
    fps() = default;
    int initialize();
    void update(double time, double frame_time);
    double get_value();
};


#endif
