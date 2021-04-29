#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <vector>
typedef std::vector <double> Row;

class Matrix
{
public:
    Matrix(int r, int c);
    void set(int r, int c, double value);
    double get(int r, int c);
    void print();
    int get_col();
    void add_row(int user_index);
    void add_col(int user_index);
    double similarity(int i, int j);
private:
    std::vector <Row> rows;
    int row, col;
};

#endif
