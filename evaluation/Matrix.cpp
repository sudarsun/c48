#include "Matrix.h"
#include "core/Utils.h"
#include <exception>

Matrix::Matrix(const int row, const int col) {
    this->mRow = row;
    this->mCol = col;
    mElements.resize(row, double_array(col));
}

Matrix::Matrix(const int row, const int col, const double scalarValue) {
    this->mRow = row;
    this->mCol = col;
    mElements.resize(row, double_array(col));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mElements[i][j] = scalarValue;
        }
    }
}

Matrix::Matrix(double_2D_array &A) {
    mRow = (int)A.size();
    mCol = (int)A[0].size();
    for (int i = 0; i < mRow; i++) {
        if (A[i].size() != mCol) {
            throw std::invalid_argument("All rows must have the same length.");
        }
    }
    this->mElements = A;
}

Matrix::Matrix(double_2D_array &A, const int row, const int col) {
    this->mElements = A;
    this->mRow = row;
    this->mCol = col;
}

Matrix::Matrix(double_array &vals, const int row) {
    this->mRow = row;
    mCol = (mRow != 0 ? (int)vals.size() / mRow : 0);
    if (mRow*mCol != vals.size()) {
        throw std::invalid_argument("Array length must be a multiple of Row.");
    }
    mElements.resize(mRow, double_array(mCol));
    for (int i = 0; i < mRow; i++) {
        for (int j = 0; j < mCol; j++) {
            mElements[i][j] = vals[i + j*mRow];
        }
    }
}

double_2D_array Matrix::getArray() const {
    return mElements;
}

double Matrix::get(const int i, const int j) const {
    return mElements[i][j];
}

void Matrix::set(const int i, const int j, const double s) {
    mElements[i][j] = s;
}

string Matrix::toString() const {
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

int Matrix::getRowDimension() const {
    return mRow;
}

int Matrix::getColumnDimension() const {
    return mCol;
}
