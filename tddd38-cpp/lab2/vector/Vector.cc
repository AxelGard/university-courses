#include <iostream>
#include "Vector.h"
#include <math.h>
#include <string>

Vector::Vector(double x, double y)
{
}

Vector::~Vector()
{
}

double Vector::length(){
    return std::sqrt(powf(this->x,2) + powf(this->y,2));
}

bool Vector::operator == (const Vector& vec){
    return this->x == vec.x && this->y == vec.y;
}

bool Vector::operator != (const Vector& vec){
    return this->x != vec.x && this->y != vec.y;
}

Vector Vector::operator + (const double& val){
    return Vector(this->x + val, this->y + val);
}

Vector Vector::operator - (const double& val){
    return Vector(this->x - val, this->y - val);
}

Vector Vector::operator - (){
    return Vector(-this->x, -this->y);
}

Vector Vector::operator * (const double& val){
    return Vector(this->x * val, this->y * val);
}

Vector Vector::operator * (const double& val){
    return Vector(val*this->x, val*this->y);
}

Vector Vector::operator / (const double& val){
    return Vector(this->x / val, this->y / val);
}

std::ostream& operator << (std::ostream& out, const Vector& vec){
    out << "(" << vec.x << "," << vec.y << ")";
    return out;
}
std::istream& operator >> (std::istream& in, const Vector& vec){
    in >> vec.x;
    in >> vec.y;
    return in;
}


Vector Vector::operator + (const Vector& vec){
    return Vector(this->x + vec.x, this->y + vec.y);
}

Vector Vector::operator - (const Vector& vec){
    return Vector(this->x - vec.x, this->y - vec.y);
}

double Vector::operator * (const Vector& vec){
    return this->x * vec.x + this->y * vec.y;
}