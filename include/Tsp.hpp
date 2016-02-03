#pragma once
#include <vector>
#include <string>
#include "Point2D.hpp"

class Tsp
{
public:
    static Tsp loadTspFile(std::string arg_filename);
    Tsp(Tsp&& other) = default;
    Tsp(const Tsp& other) = default;
    Tsp& operator=(Tsp&& other) = default;
    Tsp& operator=(const Tsp& other) = default;
    std::string Name() const;
    size_t Size() const;
    Point2D operator[](size_t index);

private:
    std::vector<Point2D> points;
    std::string name;
    size_t size;


    Tsp(size_t arg_size,std::string arg_name);
};

