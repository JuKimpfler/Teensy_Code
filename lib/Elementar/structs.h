#pragma once
#include "Param.h"
#include "enum.h"
#include <wiring.h>
#include <algorithm>

#ifndef structs_h
#define structs_h

struct Vec2{
    float x;
    float y;

    Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    float magnitude(){
        return sqrtf(x*x + y*y);
    };
    float angle(){
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
        float theta;

        void setDeg(float degAngle){
            theta = degAngle * DEG_TO_RAD;
        };
        void addDeg(float degAngle){
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
        Angle& operator+=(const float delta){
            theta += delta;
            return *this;
        };
        Angle& operator/=(const float delta){
            theta /= delta;
            return *this;
        };
        Angle& operator*=(const float delta){
            theta *= delta;
            return *this;
        };
        Angle& operator-=(const float delta){
            theta -= delta;
            return *this;
        };
        Angle& operator=(const float delta){
            theta = delta;
            return *this;
        };
        float toDeg(){
            return theta * RAD_TO_DEG;
        };
};

#endif
