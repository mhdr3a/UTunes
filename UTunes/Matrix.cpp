#include "Matrix.hpp"
using namespace std;

Matrix::Matrix(int r, int c)
{
    row = r;
    col = c;
    rows.resize(row);
    for (int i = 0; i < row; i ++)
        rows[i].resize(col);
}

void Matrix::set(int r, int c, double value)
{
    rows[r][c] = value;
}

double Matrix::get(int r, int c)
{
    return rows[r][c];
}

void Matrix::print()
{
    for (Row r : rows)
    {
        for (double value : r)
            cout << value << ' ';
        cout << '\n';
    }
}

int Matrix::get_col()
{
    return col;
}

void Matrix::add_row(int user_index)
{
    Row new_row (col, 0);
    rows.insert(rows.begin() + user_index, new_row);
    row ++;
}

void Matrix::add_col(int user_index)
{
    for (Row r : rows)
        r.insert(r.begin() + user_index, 0);
    col ++;
}

double Matrix::similarity(int i, int j)
{
    double s = 0;
    for (int x = 0; x < col; x ++)
        if (rows[i][x] == 1 && rows[j][x] == 1)
            s ++;
    return s / col;
}