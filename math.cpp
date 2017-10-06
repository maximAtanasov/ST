#include "math.hpp"

double m_abs(double arg){
    if(arg >= 0)
        return arg;
    else
        return -arg;
}

int m_min(int a, int b){
    if(a >= b)
        return b;
    else
        return a;
}

float m_min(float a, float b){
    if(a >= b)
        return b;
    else
        return a;
}

double m_min(double a, double b){
    if(a >= b)
        return b;
    else
        return a;
}

int m_max(int a, int b){
    if(a >= b)
        return a;
    else
        return b;
}

float m_max(float a, float b){
    if(a >= b)
        return a;
    else
        return b;
}

double m_max(double a, double b){
    if(a >= b)
        return a;
    else
        return b;
}
