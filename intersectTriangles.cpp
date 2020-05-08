#include "intersectTriangles.h"

using namespace std;

#include <cassert>
#include <vector>
#include <deque>
#include <set>


// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(const Point & p, const Point & q, const Point & r) {
    // for details of below formula. 
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (abs(val) < EPS) 
        return 0;  // colinear 
    return (val > 0.) ? -1 : +1; //1 : 2; // clock or counterclock wise
}


// Tests whether point 'c' is on the line segment (a, b).
// Ensure first that point c is collinear to segment (a, b) and
// then check whether c is within the rectangle formed by (a, b)
bool    pointOnLine(const Point & a, const Point & b, const Point & c) {
    return orientation(a, b, c) == 0 &&
        min(a.x, b.x) <= c.x && c.x <= max(a.x, b.x) &&
        min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y);
}


bool allEqual(bool a, bool b, bool c, bool d) {
    return a == b && b == c && c == d;
}

bool Point::isInsideTriangual(const Triangular & t) const {
    return isInside(t.a, t.b, t.c, *this);
}

Rect Rect::fromTriangle(const Point & a, const Point & b, const Point & c) {
    lt.x = min(min(a.x, b.x), c.x);
    lt.y = max(max(a.y, b.y), c.y);
    rb.x = max(max(a.x, b.x), c.x);
    rb.y = min(min(a.y, b.y), c.y);
    return *this;
}

bool Intersections::intersectByRect(const Triangular & first, const Triangular & second) {
    return Rect(first).intersects(Rect(second));
}

int getCommonEndpoints(const Point & p1, const Point & p2, const Point & p3, const Point & p4, vector<Point> & points) {

    if (p1.isEqual(p3)) {
        points.push_back(p1);
        if (p2.isEqual(p4)) points.push_back(p2);

    }
    else if (p1.isEqual(p4)) {
        points.push_back(p1);
        if (p2.isEqual(p3)) points.push_back(p2);

    }
    else if (p2.isEqual(p3)) {
        points.push_back(p2);
        if (p1.isEqual(p4)) points.push_back(p1);

    }
    else if (p2.isEqual(p4)) {
        points.push_back(p2);
        if (p1.isEqual(p3)) points.push_back(p1);
    }

    return points.size();
}


bool Intersections::intersects(Point p1, Point q1, Point p2, Point q2, Point & p3, Point & q3) {

    // general case    
    float ax = q1.x - p1.x;     // direction of line a
    float ay = q1.y - p1.y;     // ax and ay as above

    float bx = p2.x - q2.x;     // direction of line b, reversed
    float by = p2.y - q2.y;     // really -by and -by as above

    float dx = p2.x - p1.x;   // right-hand side
    float dy = p2.y - p1.y;

    float det = ax * by - ay * bx;

    if (det != 0){

        float r = (dx * by - dy * bx) / det;
        float s = (ax * dy - ay * dx) / det;

        if (r < 0 || r > 1 || s < 0 || s > 1) {
            return false;
        }

        // Both segments are a single point.
        if (p1.isEqual(q1) && q1.isEqual(p2) && p2.isEqual(q2)) {
            q3 = p1;
            return true;
        }

        vector<Point> endpoints;
        int n = getCommonEndpoints(p1, q1, p2, q2, endpoints);

        // One of the line segments is an intersecting single point.
        // NOTE: checking only n == 1 is insufficient to return early
        // because the solution might be a sub segment.
        if (n == 1 && (p1.isEqual(q2) || p2.isEqual(q2))) {
            q3 = endpoints.at(0);
            return true;
        }

        // Segments are equal.
        if (n == 2) {
            p3 = endpoints.at(0);
            q3 = endpoints.at(1);
            return true;
        }

        if (det == 0)
            return false;

        // Segment #1 is a vertical line.
        if (abs(p1.x - q1.x) < EPS) {
            float m = (q2.y - p2.y) / (q2.x - p2.x);
            float b = p2.y - m * p2.x;
            q3 = Point(p1.x, m * p1.x + b);
            return true;
        }

        // Segment #2 is a vertical line.
        if (abs(p2.x - q2.x) < EPS) {
            float m = (q1.y - p1.y) / (q1.x - p1.x);
            float b = p1.y - m * p1.x;
            q3 = Point(p2.x, m * p2.x + b);
            return true;
        }

        float m1 = (q1.y - p1.y) / (q1.x - p1.x);
        float m2 = (q2.y - p2.y) / (q2.x - p2.x);
        float b1 = p1.y - m1 * p1.x;
        float b2 = p2.y - m2 * p2.x;
        float x = (b2 - b1) / (m1 - m2);
        float y = (m1 * b2 - m2 * b1) / (m1 - m2);

        q3 = Point(x, y);
        return true;
    }
    else {
        bool collinearSegments = orientation(p1, q1, p2) == 0 && orientation(p1, q1, q2) == 0;

#ifdef _DEBUG
        // Find the four orientations needed for general and 
          // special cases 
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        bool collinearSegments1 = false;
        if (o1 == 0 && pointOnLine(p1, q1, p2)) 
            collinearSegments1 = true;
        if (o2 == 0 && pointOnLine(p1, q1, q2)) 
            collinearSegments1 = true;
        if (o3 == 0 && pointOnLine(p2, q2, p1)) 
            collinearSegments1 = true;
        if (o4 == 0 && pointOnLine(p2, q2, q1))
            collinearSegments1 = true;

        assert(collinearSegments == collinearSegments1);
#endif

        // The intersection will be a sub-segment of the two
        // segments since they overlap each other.
        if (collinearSegments) {

            // Segment #2 is enclosed in segment #1
            if (pointOnLine(p1, q1, p2) && pointOnLine(p1, q1, q2)) {
                p3 = p2;
                q3 = q2;
                return true;
            }

            // Segment #1 is enclosed in segment #2
            if (pointOnLine(p2, q2, p1) && pointOnLine(p2, q2, q1)) {
                p3 = p1;
                q3 = q1;
                return true;
            }

            // The subsegment is part of segment #1 and part of segment #2.
            // Find the middle points which correspond to this segment.
            Point midPoint1 = pointOnLine(p1, q1, p2) ? p2 : q2;
            Point midPoint2 = pointOnLine(p2, q2, p1) ? p1 : q1;

            // There is actually only one middle point!
            if (midPoint1.isEqual(midPoint2)) {
                p3 = midPoint1;
                return true;
            }

            p3 = midPoint1;
            q3 = midPoint2;
            return true;
        }
    }

    return false;
}

float distance_rough(Point * first, Point * second) {
   // return sqrt(pow(second->x - first->x, 2) + pow(second->y - first->y, 2));
   return (second->x - first->x)*(second->x - first->x) + (second->y - first->y)*(second->y - first->y);
}
//(base->x - first->x)*(base->x - first->x) + (base->y - first->y)*(base->y - first->y)
//(base->x - second->x)*(base->x - second->x) + (base->y - second->y)*(base->y - second->y)
bool which_is_near(Point * base, Point * first, Point * second) {
    return distance_rough(base, first) < distance_rough(base, second) ? true : false;
}
 
void appen_to_result(const Point & p, vector<Point> & points) {
    vector<Point>::const_iterator findIter = find(points.cbegin(), points.cend(), p);
    if (findIter == points.cend()) {
        points.push_back(p);
//         if (points.size() > 2) {
//             if (orientation(points[points.size() - 3], points[points.size() - 2], points[points.size() - 1])) {
//                 swap(points[points.size() - 3], points[points.size() - 1]);
//             }
//         }
    }
}

void appen_to_result1(const Point & p, deque<Point> & points) {
    deque<Point>::const_iterator findIter = find(points.cbegin(), points.cend(), p);
    if (findIter == points.cend())
        points.push_back(p);
}

void appen_to_result_set(const Point & p, set<Point> & points) {
//     if (points.find(p)==points.end()) {
//         points.insert(p);
//     }
}

void appen_to_result_if_point_is_inside(const Point & p, vector<Point> & points) {
    if (p.inside)
        appen_to_result(p, points);
}
void append_part_points_to_result(Point * all_points, vector<Point> & points, Point * base) {
    // find near point to a
    int indx_first = -1, indx_second = -1;
    for (int n = 0; n < 6; n++) {
        if (!all_points[n].isEmpty()) {
            if (indx_first < 0)
                indx_first = n;
            else if (!all_points[n].isEqual(all_points[indx_first])) {
                indx_second = n;
                break;
            }
        }
    }
    if (indx_second >= 0) {
        if (which_is_near(base, &all_points[indx_second], &all_points[indx_first])) {
            swap(all_points[indx_second], all_points[indx_first]);
        }
    }

    appen_to_result(all_points[indx_first], points);

    if (indx_second>=0) {
        appen_to_result(all_points[indx_second], points);
    }
}


bool Intersections::triangulate(const Triangular & tr1, const Triangular & tr2, std::vector<Triangular> & result){
    result.clear();

    if (!intersectByRect(tr1, tr2)) {
        return false;
    }
    Triangular t1 = tr1;
    Triangular t2 = tr2;
    t1.a.inside = t1.a.isInsideTriangual(t2);
    t1.b.inside = t1.b.isInsideTriangual(t2);
    t1.c.inside = t1.c.isInsideTriangual(t2);
    t2.a.inside = t2.a.isInsideTriangual(t1);
    t2.b.inside = t2.b.isInsideTriangual(t1);
    t2.c.inside = t2.c.isInsideTriangual(t1);


    if (t1.a.inside && t1.b.inside && t1.c.inside) {
        result.push_back(t1);
        return true;
    }
    else if (!t1.a.inside && !t1.b.inside && !t1.c.inside) {
        if (t2.a.inside && t2.b.inside && t2.c.inside) {
            result.push_back(t2);
            return true;
        }
    }

    vector<Point> points;
    // deque<Point> points;
    // set<Point> points;

    Triangular first = t1.clockwised();
    Triangular second = t2.clockwised();
         
    auto intersectsPlus = [&](const Point & p, const Point & q, const Point & a, const Point & b, const Point & c) {
        Point all_points[6]; int i = 0; int n = 0;
        
        if (intersects(p, q, a, b, all_points[i++], all_points[i++])) {
            appen_to_result_if_point_is_inside(a, points);
            n = 1;
        }
        if (intersects(p, q, b, c, all_points[i++], all_points[i++])) {
            appen_to_result_if_point_is_inside(b, points);
            n |= 2;
        }
        if (n < 3 && intersects(p, q, c, a, all_points[i++], all_points[i++])) {
            appen_to_result_if_point_is_inside(c, points);
            n |= 4;
        }

        if (n >= 1) {
            appen_to_result_if_point_is_inside(p, points);
            append_part_points_to_result(all_points, points, (Point*)&p);
        
            if (n & 1)
                appen_to_result_if_point_is_inside(b, points);
            if (n & 2)
                appen_to_result_if_point_is_inside(c, points);
            if (n & 4)
                appen_to_result_if_point_is_inside(a, points);
        }

    };


    intersectsPlus(first.a, first.b, second.a, second.b, second.c);

    intersectsPlus(first.b, first.c, second.a, second.b, second.c);

    intersectsPlus(first.c, first.a, second.a, second.b, second.c);

    //
    if (points.size() == 0) {
        return false;
    }
    else if (points.size() < 3) {
        return false;
    }
    else if (points.size() == 3) {
        result.push_back(Triangular(points.at(0), points.at(1), points.at(2)));
        return true;
    }
    else {
        int cnt = points.size();
        for (int i = 1; i + 1 < cnt; i++) {
            result.push_back(Triangular(points.at(0), points.at(i), points.at(i + 1)));
        }
    }

    return true;

}

Triangular Triangular::clockwised() const{
    Triangular t;
    int o = orientation(a, b, c);
    if (o == 0) {
        assert(false);
    }
    else if(o < 0.){
        t = *this;
    }
    else {
        t.a = c;
        t.b = b;
        t.c = a;
    }
    return t;
}
