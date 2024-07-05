//
// Created by Till Wegener on 05.07.24.
//

#ifndef NSIM_SCIVEC3_H
#define NSIM_SCIVEC3_H

#include "cmath"

struct SciVec3
{
    double x;
    double y;
    double z;

    SciVec3() : x(0), y(0), z(0) {}

    SciVec3(double x, double y, double z) : x(x), y(y), z(z) {}

    SciVec3 operator+(const SciVec3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    SciVec3 operator+=(const SciVec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    SciVec3 operator-(const SciVec3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    SciVec3 operator*(double scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    SciVec3 operator/(double scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    double length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    SciVec3 normalized() const
    {
        double len = length();
        return {x / len, y / len, z / len};
    }
};

#endif //NSIM_SCIVEC3_H
