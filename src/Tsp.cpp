#include "Tsp.hpp"
#include <fstream>
#include <sstream>
Tsp Tsp::loadTspFile(std::string arg_filename)
{
    using namespace std;

    ifstream in_file(arg_filename.c_str());
    string name;
    size_t size;

    while (!in_file.eof()) {
        string one_line;
        getline(in_file, one_line);
        stringstream ss(one_line);

        string command , split;
        ss >> command >> split;

        if (command == "NAME") {
            ss >> name;
        } else if (command == "DIMENSION") {
            ss >> size;
        } else if (command == "NODE_COORD_SECTION") {
            Tsp tsp(size,name);

            for (size_t i = 0; i < size; i++) {
                size_t index;
                double x, y;

                in_file >> index >> x >> y;
                tsp.points[i] = Point2D(x, y);
            }

            return tsp;
        }
    }

    return Tsp(0,"");
}

std::string Tsp::Name() const
{
    return name;
}

size_t Tsp::Size() const
{
    return size;
}

Point2D Tsp::operator[](size_t index)
{
    return points[index];
}

Tsp::Tsp(size_t arg_size, std::string arg_name)
    :size(arg_size),name(arg_name),points(arg_size)
{
}
