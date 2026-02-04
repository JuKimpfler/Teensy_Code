/*#pragma once
#include "Param.h"
#include "enum.h"
#include <wiring.h>
#include <algorithm>

#ifndef structs_h
#define structs_h

struct Vec2{
    double x;
    double y;

    Vec2(double x = 0, double y = 0) : x(x), y(y) {}

    double magnitude(){
        return sqrtf(x*x + y*y);
    };
    double angle(){
        return atanf(y/x);
    };
    Vec2 operator+(Vec2& other){
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2 operator-(Vec2& other){
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

struct Angle{
        double theta;

        void setDeg(double degAngle){
            theta = degAngle * DEG_TO_RAD;
        };
        void addDeg(double degAngle){
            theta += degAngle * DEG_TO_RAD;
        };
        Angle& operator+=(const Angle& other){
            theta += other.theta;
            return *this;
        };
        Angle& operator/=(const Angle& other){
            theta /= other.theta;
            return *this;
        };
        Angle& operator*=(const Angle& other){
            theta *= other.theta;
            return *this;
        };
        Angle& operator-=(const Angle& other){
            theta -= other.theta;
            return *this;
        };
        Angle& operator=(const Angle& other){
            theta = other.theta;
            return *this;
        };
        Angle& operator+=(const double delta){
            theta += delta;
            return *this;
        };
        Angle& operator/=(const double delta){
            theta /= delta;
            return *this;
        };
        Angle& operator*=(const double delta){
            theta *= delta;
            return *this;
        };
        Angle& operator-=(const double delta){
            theta -= delta;
            return *this;
        };
        Angle& operator=(const double delta){
            theta = delta;
            return *this;
        };
        double toDeg(){
            return theta * RAD_TO_DEG;
        };
};

#endif
*/