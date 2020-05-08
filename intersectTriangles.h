#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <deque>
#include <vector>

#define EPS 0.00001
#define isAlmostZero(uNumerator) (fabs(uNumerator) < EPS)


struct  Triangular;

struct Point {
    float x;
    float y;
    bool inside;
    Point() :x(FLT_MAX), y(FLT_MAX), inside(false) {}
    Point(float x, float y) : x(x), y(y), inside(false) {}
    Point(const Point & other) {
        x = other.x;
        y = other.y;
        inside = other.inside;
    }
    Point & operator = (const Point & other) {
        x = other.x;
        y = other.y;
        inside = other.inside;
        return *this;
    }

    bool operator == (const Point & other) const {
        return isEqual(other);
     }

    bool isEmpty() const {
        return x == FLT_MAX && y == FLT_MAX;
    }
    bool isEqual( const Point & other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    bool isInsideTriangual(const Triangular & t) const;

    bool isInsideTriangual(const Point & a, const Point & b, const Point & c) {
        return isInside(a, b, c, *this);
    }
    /* A function to check whether point P(x, y) lies inside the triangle formed
       by A(x1, y1), B(x2, y2) and C(x3, y3) */
    static bool isInside(const Point & a, const Point & b, const Point & c, const Point & p) {
        /* Calculate area of triangle ABC */
        double A = area(a, b, c);

        /* Calculate area of triangle PBC */
        double A1 = area(p, b, c);

        /* Calculate area of triangle PAC */
        double A2 = area(a, p, c);

        /* Calculate area of triangle PAB */
        double A3 = area(a, b, p);

        /* Check if sum of A1, A2 and A3 is same as A */
        return (A == A1 + A2 + A3);
    }

    static double area(const Point & a, const Point & b, const Point & c) {
        return abs((a.x*(b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
    }

};


struct Triangular {
    Triangular() {};
    Triangular(const Point & a, const Point & b, const Point & c) {
        this->a = a;
        this->b = b;
        this->c = c;
    };
    Triangular(const Triangular & other) :a(other.a), b(other.b), c(other.c) { };
    Triangular & operator = (const Triangular & other) {
        a = other.a;
        b = other.b;
        c = other.c;
        return *this;
    }
    void setA(const Point & a) {
        this->a = a;
    }

    bool isEqual(const Triangular & other) const {
        return (a.isEqual(other.a) && b.isEqual(other.b) && c.isEqual(other.c)) ||
            (a.isEqual(other.a) && b.isEqual(other.c) && c.isEqual(other.b)) ||
            (a.isEqual(other.b) && b.isEqual(other.c) && c.isEqual(other.a)) ||
            (a.isEqual(other.b) && b.isEqual(other.a) && c.isEqual(other.c)) ||
            (a.isEqual(other.c) && b.isEqual(other.a) && c.isEqual(other.b)) ||
            (a.isEqual(other.c) && b.isEqual(other.b) && c.isEqual(other.a)) ;
    }

    bool isNull() {
        return a.x == 0 && a.y == 0 && b.x == 0 && b.y == 0 && c.x == 0 && c.y == 0;
    }

    Triangular clockwised() const;
    Point a, b, c;
};


struct Rect {
    Point lt;
    Point rb;
    Rect() :lt(), rb() {
    }
    Rect(const Point & lt, const Point & rb) : lt(lt), rb(rb) {
    }
    Rect(const Rect & other) : lt(other.lt), rb(other.rb) {
    }

    Rect(const Triangular & t) {
        fromTriangle(t);
    }
    Rect & operator = (const Rect & other) {
        lt = other.lt;
        rb = other.rb;
        return *this;
    }

    bool intersects(const Rect & other) {
        // If one rectangle is on left side of other 
        if (lt.x >= other.rb.x || other.lt.x >= rb.x)
            return false;

        // If one rectangle is above other 
        if (lt.y <= other.rb.y || other.lt.y <= rb.y)
            return false;

        return true;
    }

    Rect fromTriangle(const Triangular & t) {
        return fromTriangle(t.a, t.b, t.c);
    }

    Rect fromTriangle(const Point & a, const Point & b, const Point & c);

};

//-------------------------------------------------------------------------
class Intersections {

public:
    //-------------------------------------------------------------------------
    // input: two triangles A, B
    // steps:
    // 1. check if intersected rectangles of triangles, if No then End 
    // 2. check if each point of triangle A inside triangle B and oversize check B inside A, 
    //   2.1 if all points one triangle inside other one then result is equal inside triangle
    //   2.2 oversize continue to collect intersection points 
    // 3. check by enumerate each edge of triangle A with all edges of triangle B, 
    //    after getting intersection point (or points in case collinear both lines), insert it into result polygon 
    // 4. if polygon has more that 3 points then need to make triangulation with that 

    bool triangulate(const Triangular & t1, const Triangular & t2, std::vector<Triangular> & result);

private:
    bool intersects(Point p1, Point q1, Point p2, Point q2, Point & p3, Point & q3);
    bool intersectByRect(const Triangular & first, const Triangular & second);

};

