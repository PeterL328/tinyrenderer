//
// Created by peter on 2020-02-27.
//
#include <vector>

#include "geometry.h"


Matrix::Matrix(int r, int c) {
    m = std::vector<std::vector<float>>(r, std::vector<float>(c, 0.f));
    rows = r;
    cols = c;
}

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix res(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        res[i][i] = 1.f;
    }
    return res;
}

Matrix Matrix::transpose() {
    Matrix res(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[j][i] = m[i][j];
        }
    }
    return res;
}

Matrix Matrix::inverse() {
    return Matrix();
}

std::ostream &operator<<(std::ostream &s, Matrix &m) {
    for (int row = 0; row < m.nrows(); row++) {
        for (int col = 0; col < m.ncols(); col++) {
            s << m[row][col];
            if (col < m.cols - 1) {
                s << "\t";
            }
        }
        s << "\n";
    }
    return s;
}


const std::vector<float> &Matrix::operator[](int i) const {
    assert(i >= 0 && i < rows);
    return m[i];
}

std::vector<float> &Matrix::operator[](int i) {
    return const_cast<std::vector<float> &>(static_cast<const Matrix &>(*this)[i]);
}


Matrix Matrix::operator*(const Matrix &mat) {
    assert(cols == mat.rows);
    Matrix res(rows, mat.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            float temp_sum = 0.f;
            for (int k = 0; k < cols; k++) {
                temp_sum += m[i][k] * mat[k][j];
            }
            res[i][j] = temp_sum;
        }
    }
    return res;
}
