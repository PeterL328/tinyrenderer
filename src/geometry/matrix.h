//
// Created by peter on 2020-03-21.
//

#ifndef TINYRENDERER_MATRIX_H
#define TINYRENDERER_MATRIX_H

#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>


const int DEFAULT_M_SIZE = 4;

namespace Geometry {
    class Matrix {
        std::vector<std::vector<float>> m;
        int rows, cols;
    public:
        explicit Matrix(int r = DEFAULT_M_SIZE, int c = DEFAULT_M_SIZE);

        inline int nrows();

        inline int ncols();

        static Matrix identity(int dimensions);

        Matrix transpose();

        Matrix inverse();

        const std::vector<float> &operator[](int i) const;

        std::vector<float> &operator[](int i);

        Matrix operator*(const Matrix &mat) const;

        Matrix operator*(const Matrix &mat);

        friend std::ostream &operator<<(std::ostream &s, Matrix &m);
    };
}
#endif //TINYRENDERER_MATRIX_H
