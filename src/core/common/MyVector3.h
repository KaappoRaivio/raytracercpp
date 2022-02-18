//
// Created by kaappo on 11.9.2021.
//

#pragma once


#include <ostream>
#include <iostream>
#include "../../../lib/Eigen/Dense"
#include <glm/glm.hpp>

class MyVector3 {
private:
    double i, j, k;

public:
    constexpr MyVector3 (double i, double j, double k) : i{i}, j{j}, k{k} {

    };

    MyVector3 operator+ (const MyVector3& other) const;
    MyVector3 operator|| (const MyVector3& other) const;
    MyVector3 operator- () const;
    MyVector3 operator- (const MyVector3& other) const;
    MyVector3 operator* (double other) const;
    double operator* (const MyVector3& other) const;
    MyVector3 operator/ (double other) const;
    explicit operator bool () const;
    double operator [] (int index) const;

    bool operator== (const MyVector3& rhs) const;

    bool operator!= (const MyVector3& rhs) const;

    MyVector3 rotateInsideCone (double radius) const;
    MyVector3 reflection (const MyVector3& axis) const;
    MyVector3 inTermsOfComponents (const MyVector3& i, const MyVector3& j, const MyVector3& k) const;

    MyVector3 rotate (double yaw, double pitch) const;
    MyVector3 cross (const MyVector3& other) const;
    double length () const;

    double squared () const;
    MyVector3 normalize () const;

    double atan2 () const;

    double asin () const;

    friend std::ostream& operator<< (std::ostream& os, const MyVector3& vector3);

    double getI () const;
    double getJ () const;
    double getK () const;

    static MyVector3 randomInsideUnitSphere ();


    double dot (const MyVector3& vector3) const;
};

namespace MyVector3s {
    constexpr MyVector3 UP{0, 0, 1};
    constexpr MyVector3 OUT{0, 1, 0};
    constexpr MyVector3 SIDE{1, 0, 0};
}