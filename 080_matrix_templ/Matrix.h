#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>
#include <iostream>
#include <vector>
#include <cstdlib>


//YOUR CODE GOES HERE!
template <typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  std::vector<std::vector<T> > rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows() {}
  Matrix(int r, int c) : numRows(r), numColumns(c), rows(r, std::vector<T>(c)) {}
  Matrix(const Matrix & rhs) : numRows(rhs.numRows), numColumns(rhs.numColumns), rows(rhs.rows) {}
  ~Matrix() {}

  Matrix & operator=(const Matrix & rhs) {
    if (this != &rhs) {
      numRows = rhs.numRows;
      numColumns = rhs.numColumns;
      rows = rhs.rows;
    }
    return *this;
  }

  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }

  const std::vector<T> & operator[](int index) const {
    assert(index >= 0 && index < numRows);
    return rows[index];
  }

  std::vector<T> & operator[](int index) {
    assert(index >= 0 && index < numRows);
    return rows[index];
  }

  bool operator==(const Matrix & rhs) const {
    if (numRows != rhs.numRows || numColumns != rhs.numColumns) {
      return false;
    }
    for (int i = 0; i < numRows; ++i) {
      if (rows[i] != rhs.rows[i]) {
        return false;
      }
    }
    return true;
  }

  Matrix operator+(const Matrix & rhs) const {
    assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
    Matrix result(numRows, numColumns);
    for (int i = 0; i < numRows; ++i) {
      for (int j = 0; j < numColumns; ++j) {
        result[i][j] = rows[i][j] + rhs.rows[i][j];
      }
    }
    return result;
  }
};

template <typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); ++i) {
    s << "{";
    for (int j = 0; j < rhs.getColumns(); ++j) {
      s << rhs[i][j];
      if (j < rhs.getColumns() - 1) {
        s << ", ";
      }
    }
    s << "}";
    if (i < rhs.getRows() - 1) {
      s << ",\n";
    }
  }
  s << " ]";
  return s;
}

#endif
