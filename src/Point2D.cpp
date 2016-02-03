#include "Point2D.hpp"
#include <cmath>


Point2D::Point2D(double argX, double argY)
    : x(argX),y(argY)
{
}

double Point2D::X()
{
    return x;
}

double Point2D::Y()
{
    return y;
}

double Point2D::Dist(const Point2D& a, const Point2D& b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
