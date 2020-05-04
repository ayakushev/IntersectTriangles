// t1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <cassert>


#include "intersectTriangles.h"


using namespace std;

int main() {

    vector< pair<Triangular, Triangular> > tests;
    vector< Triangular> answer;

    // 1 
    tests.push_back( pair<Triangular, Triangular>(
        Triangular(Point(0,0),Point(0,10),Point(10,10)), 
        Triangular(Point(0,0),Point(0,5),Point(10,10))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 5), Point(10, 10)));
    // 2 
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 10), Point(10, 10)), 
        Triangular(Point(0, -10), Point(0, 5), Point(10, 10))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 5), Point(10, 10)));

    // 3 
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 5), Point(0, 10), Point(10, 10)),
        Triangular(Point(0, -10), Point(0, 5), Point(10, 10))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 0), Point(0, 0)));

    // 4 
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 5), Point(5, 0)),
        Triangular(Point(0.1, 0), Point(-1, 0), Point(-2, 0))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 0), Point(0, 0)));

    // 5
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 5), Point(5, 0)),
        Triangular(Point(1, 1), Point(1, 2), Point(2, 1))));
    answer.push_back(
        Triangular(Point(1, 1), Point(1, 2), Point(2, 1)));

    // 6
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 5), Point(5, 0)),
        Triangular(Point(-1, -1), Point(-1, 6), Point(7, -1))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 5), Point(5, 0)));

    // 7
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 5), Point(5, 0)),
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)));

     // 8 parallels 6 cases
    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(1, 0), Point(-2, 0), Point(-2, 3))));
    answer.push_back(
        Triangular(Point(0, 0), Point(1, 0), Point(0, 1)));

    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(2, 0), Point(5, 0), Point(5, 3))));
    answer.push_back(
        Triangular(Point(2, 0), Point(3, 0), Point(2.5, 0.5)));

    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(0, 2), Point(0, 5), Point(3, 5))));
    answer.push_back(
        Triangular(Point(0, 2), Point(0, 3), Point(0.5, 2.5)));

    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(0, 1), Point(0, -2), Point(2, -2))));
    answer.push_back(
        Triangular(Point(0, 0), Point(0, 1), Point(2./3., 0)));

    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(2, 1), Point(5, -2), Point(2, -2))));
    answer.push_back(
        Triangular(Point(2, 0), Point(2, 1), Point(3, 0)));

    tests.push_back(pair<Triangular, Triangular>(
        Triangular(Point(0, 0), Point(0, 3), Point(3, 0)),
        Triangular(Point(1, 2), Point(-2, 5), Point(-2, 2))));
    answer.push_back(
        Triangular(Point(0, 2), Point(0, 3), Point(1, 2)));


    {
        for (int n = 0; n < 1000; n++)
            for (unsigned i = 0; i < tests.size(); i++) {
                vector<Triangular> result;
                Intersections tool;
                if (tool.triangulate(tests[i].first, tests[i].second, result)) {
                    assert(result[0].isEqual(answer.at(i)));
                }
                else {
                    if (!answer.at(i).isNull())
                        assert(false);
                }
            }
    }

    return 0;
};



