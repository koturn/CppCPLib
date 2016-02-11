#ifndef ARRAY2D_HPP
#define ARRAY2D_HPP

#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>


template<typename ElmType>
class Matrix {
public:
  typedef std::size_t size_type;
private:
  size_type nRow;
  size_type nCol;
  ElmType* data;

  static Matrix<ElmType>
  add(Matrix<ElmType>& matZ, const Matrix<ElmType>& matX, const Matrix<ElmType>& matY)
  {
    for (size_type i = 0; i < matZ.nRow; i++) {
      for (size_type j = 0; j < matZ.nCol; j++) {
        matZ[i][j] = matX[i][j] + matY[i][j];
      }
    }
    return matZ;
  }

  static Matrix<ElmType>
  sub(Matrix<ElmType>& matZ, const Matrix<ElmType>& matX, const Matrix<ElmType>& matY)
  {
    for (size_type i = 0; i < matZ.nRow; i++) {
      for (size_type j = 0; j < matZ.nCol; j++) {
        matZ[i][j] = matX[i][j] - matY[i][j];
      }
    }
    return matZ;
  }

  static Matrix<ElmType>
  mul(Matrix<ElmType>& matZ, const Matrix<ElmType>& matX, const Matrix<ElmType>& matY)
  {
    for (size_type i = 0; i < matZ.nRow; i++) {
      for (size_type k = 0; k < matX.col; k++) {
        ElmType p = 0;
        for (size_type j = 0; j < matZ.nCol; j++) {
          p += matX[j][k] * matY[k][j];
        }
        matZ[i][j] = p;
      }
    }
    return matZ;
  }

  static Matrix<ElmType>
  mul(Matrix<ElmType>& matZ, const Matrix<ElmType>& matX, const ElmType& y)
  {
    for (size_type i = 0; i < matZ.nRow; i++) {
      for (size_type j = 0; j < matZ.nCol; j++) {
        matZ[i][j] = matX[i][j] * y;
      }
    }
    return matZ;
  }

  static Matrix<ElmType>
  div(Matrix<ElmType>& matZ, const Matrix<ElmType>& matX, const ElmType& y)
  {
    for (size_type i = 0; i < matZ.nRow; i++) {
      for (size_type j = 0; j < matZ.nCol; j++) {
        matZ[i][j] = matX[i][j] / y;
      }
    }
    return matZ;
  }

public:
  static Matrix<ElmType>
  Identity(size_type nRowCol)
  {
    Matrix<ElmType> iMat(nRowCol, nRowCol);
    for (size_type i = 0; i < nRowCol; i++) {
      iMat[i][i] = 1;
    }
    return iMat;
  }

  Matrix(size_type nRow, size_type nCol) :
    nRow(nRow), nCol(nCol), data(new ElmType[nRow * nCol])
  {}

  Matrix(const Matrix<ElmType>& that) :
    nRow(that.nRow), nCol(that.nCol), data(new ElmType[nRow * nCol])
  {
    std::memcpy(data, that.data, nRow * nCol * sizeof(ElmType));
  }

  ~Matrix()
  {
    delete[] data;
  }

  void
  fill(const ElmType& value)
  {
    std::fill_n(data, nRow * nCol, value);
  }

  Matrix<ElmType>
  add(const Matrix<ElmType>& that) const
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    return add(result, *this, that);
  }

  Matrix<ElmType>
  add_(const Matrix<ElmType>& that)
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    return add(*this, *this, that);
  }

  Matrix<ElmType>
  sub(const Matrix<ElmType>& that) const
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    return sub(result, *this, that);
  }

  Matrix<ElmType>
  sub_(const Matrix<ElmType>& that)
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    return sub(*this, *this, that);
  }

  Matrix<ElmType>
  mul(const Matrix<ElmType>& that) const
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    return mul(result, *this, that);
  }

  Matrix<ElmType>
  mul(const ElmType& that) const
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    return mul(result, *this, that);
  }

  Matrix<ElmType>
  mul_(const Matrix<ElmType>& that)
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    mul(result, *this, that);
    *this = result;
    return *this;
  }

  Matrix<ElmType>
  mul_(const ElmType& that)
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    return mul(*this, *this, that);
  }

  Matrix<ElmType>
  div(const ElmType& that) const
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    Matrix<ElmType> result(this->nRow, this->nCol);
    return div(result, *this, that);
  }

  Matrix<ElmType>
  div_(const ElmType& that)
  {
    assert(this->nRow == that.nRow && this->nCol == that.nCol);
    return div(*this, *this, that);
  }

  ElmType
  det() const
  {
    assert(this->nRow == this->nCol);
    for (size_type i = 0; i < this->nRow; i++){
      for (size_type j = 0; j < this->nCol; j++){
        if (i < j){
          ElmType tmp = (*this)[j][i] / (*this)[i][i];
          for (size_type k = 0; k < this->nRow; k++){
            (*this)[j][k] -= (*this)[i][k] * tmp;
          }
        }
      }
    }
    ElmType det = 1;
    for (size_type i = 0; i < this->nRow; i++){
      det *= (*this)[i][i];
    }
    return det;
  }

  Matrix<ElmType>
  transpose() const
  {
    Matrix<ElmType> tMat(this->nCol, this->nRow);
    for (size_type i = 0; i < nRow; i++) {
      for (size_type j = 0; j < nCol; j++) {
        tMat[j][i] = (*this)[i][j];
      }
    }
    return tMat;
  }

  Matrix<ElmType>
  inverse() const
  {
    assert(this->nRow == this->nCol);
    Matrix orgMat = *this;
    Matrix invMat = Identity(this->nRow);
    for (size_type i = 0; i < invMat.nRow; i++) {
      if (orgMat[i][i] == 0) {
        return Matrix(nRow, nCol);
      }
      ElmType tmp = 1 / orgMat[i][i];
      for (size_type j = 0; j < invMat.nRow; j++){
        orgMat[i][j] *= tmp;
        invMat[i][j] *= tmp;
      }
      for (size_type j = 0; j < invMat.nRow; j++){
        if (i != j) {
          for (size_type k = 0; k < invMat.nRow; k++) {
            orgMat[j][k] -= orgMat[i][k] * orgMat[j][i];
            invMat[j][k] -= invMat[i][k] * orgMat[j][i];
          }
        }
      }
    }
    return invMat;
  }

  ElmType&
  at(size_type y, size_type x) const
  {
    assert(y < nRow && x < nCol);
    return data[y * nCol + x];
  }

  size_type
  getNRow() const
  {
    return nRow;
  }

  size_type
  getNCol() const
  {
    return nCol;
  }

  Matrix<ElmType>
  operator+(const Matrix<ElmType>& that) const
  {
    return this->add(that);
  }

  Matrix<ElmType>
  operator-(const Matrix<ElmType>& that) const
  {
    return this->sub(that);
  }

  Matrix<ElmType>
  operator*(const Matrix<ElmType>& that) const
  {
    return this->mul(that);
  }

  Matrix<ElmType>
  operator+=(const Matrix<ElmType>& that)
  {
    return this->add_(that);
  }

  Matrix<ElmType>
  operator-=(const Matrix<ElmType>& that)
  {
    return this->sub_(that);
  }

  Matrix<ElmType>
  operator*=(const Matrix<ElmType>& that)
  {
    return this->mul_(that);
  }

  ElmType*
  operator[](size_type row) const
  {
    return &data[row * nCol];
  }

  Matrix<ElmType>&
  operator=(const Matrix<ElmType>& that)
  {
    nRow = that.nRow;
    nCol = that.nCol;
    data = new ElmType[nRow * nCol];
    for (int i = 0; i < nRow; i++) {
      for (int j = 0; j < nCol; j++) {
        (*this)[i][j] = that[i][j];
      }
    }
    return *this;
  }

  friend Matrix<ElmType>
  operator*(const Matrix<ElmType>& this_, double n)
  {
    return this_.mul(n);
  }

  friend Matrix<ElmType>
  operator*(double n, const Matrix<ElmType>& this_)
  {
    return this_.mul(n);
  }

  friend Matrix<ElmType>
  operator/(const Matrix<ElmType>& this_, double n)
  {
    return this_.div(n);
  }

  friend Matrix<ElmType>
  operator*=(const Matrix<ElmType>& this_, double n)
  {
    return this_.mul_(n);
  }

  friend Matrix<ElmType>
  operator/=(const Matrix<ElmType>& this_, double n)
  {
    return this_.div_(n);
  }

  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const Array2D<ElmType>& this_)
  {
    os << "{\n";
    for (size_type i = 0; i < this_.nRow; i++) {
      os << "  {";
      for (size_type j = 0, jMax = this_.nCol - 1; j < jMax; j++) {
        os << this_[i][j] << ", ";
      }
      os << this_[i][this_.nCol - 1] << "}\n";
    }
    os << "}";
    return os;
  }
};  // class Matrix


#endif  // MATRIX_HPP
