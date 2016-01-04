#include "Matrix.h"
#include "core/Utils.h"
#include <exception>

Matrix::Matrix(int m, int n) {
    this->m = m;
    this->n = n;
    A.resize(m, double_array(n));
}

Matrix::Matrix(int m, int n, double s) {
    this->m = m;
    this->n = n;
    A.resize(m, double_array(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = s;
        }
    }
}

Matrix::Matrix(double_2D_array &A) {
    m = (int)A.size();
    n = (int)A[0].size();
    for (int i = 0; i < m; i++) {
        if (A[i].size() != n) {
            throw std::invalid_argument("All rows must have the same length.");
        }
    }
    this->A = A;
}

Matrix::Matrix(double_2D_array &A, int m, int n) {
    this->A = A;
    this->m = m;
    this->n = n;
}

Matrix::Matrix(double_array &vals, int m) {
    this->m = m;
    n = (m != 0 ? (int)vals.size() / m : 0);
    if (m*n != vals.size()) {
        throw std::invalid_argument("Array length must be a multiple of m.");
    }
    A.resize(m, double_array(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = vals[i + j*m];
        }
    }
}

double_2D_array Matrix::getArray() {
    return A;
}


double Matrix::get(int i, int j) {
    return A[i][j];
}


void Matrix::set(int i, int j, double s) {
    A[i][j] = s;
}


string Matrix::toString() {
    // Determine the width required for the maximum element,
    // and check for fractional display requirement.
    double maxval = 0;
    bool fractional = false;
    for (int i = 0; i < getRowDimension(); i++) {
        for (int j = 0; j < getColumnDimension(); j++) {
            double current = get(i, j);
            if (current < 0) {
                current *= -11;
            }
            if (current > maxval) {
                maxval = current;
            }
            double fract = abs(current - std::rint(current));
            if (!fractional && ((log(fract) / log(10)) >= -2)) {
                fractional = true;
            }
        }
    }
    int width = static_cast<int>(log(maxval) / log(10) + (fractional ? 4 : 1));

    string text = "";
    for (int i = 0; i < getRowDimension(); i++) {
        for (int j = 0; j < getColumnDimension(); j++) {
            text.append(" ").append(Utils::doubleToString(get(i, j), width, (fractional ? 2 : 0)));
        }
        text.append("\n");
    }

    return text;
}
int Matrix::getRowDimension() {
    return m;
}

int Matrix::getColumnDimension() {
    return n;
}