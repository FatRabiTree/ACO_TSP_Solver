#pragma once
class Point2D
{
public:
    Point2D(Point2D&& other) = default;
    Point2D(const Point2D& other) = default;
    Point2D& operator=(Point2D&& other) = default;
    Point2D& operator=(const Point2D& other) = default;
    Point2D(double argX = 0.0,double argY = 0.0);
    double X();
    double Y();
    static double Dist(const Point2D& a, const Point2D& b);
private:
    double x;
    double y;
};

