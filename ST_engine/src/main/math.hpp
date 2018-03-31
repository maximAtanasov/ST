#ifndef MATH_DEF
#define MATH_DEF

//TODO: Must be completely redone

    //abs functions
    inline int m_abs(int value){
        auto temp = static_cast<unsigned int>(value >> 31);
        value ^= temp;
        value += temp & 1;
        return value;
    }

    //this is supposed to be faster than a simple branch, but for some reason isn't, so idk, it isn't really used anywhere so whatever
    inline float m_fabs(float abs){
        void* ptr = &abs;
        int x = *(int*)ptr;
        x = x & 0x7FFFFFFF;
        ptr = &x;
        return *(float*)ptr;
    }

    double m_abs(double abs);

    //min functions
    int m_min(int a, int b);
    float m_min(float a, float b);
    double m_min(double a, double b);

    //max functions
    int m_max(int a, int b);
    float m_max(float a, float b);
    double m_max(double a, double b);

    //mod function - only for positive integers (and yes, it is faster than %, I've tested it)
    inline int m_mod(int a, int b){
        return a >= b ? a % b : a;
    }

#endif
