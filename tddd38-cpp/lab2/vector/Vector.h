#pragma once

class Vector
{
private:
    /* data */

public:
    Vector(double x=0, double y=0);
    ~Vector();

    double x;
    double y;

    double length();

    //compare
    bool operator == (const Vector& vec);
    bool operator != (const Vector& vec);
    
    // val op 
    Vector operator + (const double& val);
    Vector operator - (const double& val);
    Vector operator - ();
    Vector operator * (const double& val);
    Vector operator * (const double& val);
    Vector operator / (const double& val);
    friend std::ostream& operator << (std::ostream& out, const Vector& vec);
    friend std::istream& operator >> (std::istream& in, const Vector& vec);

    // interatior op 
    Vector operator + (const Vector& vec);
    Vector operator - (const Vector& vec);
    double operator * (const Vector& vec);




};

