#ifndef ARRAY2D_HPP
#define ARRAY2D_HPP

#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <memory>


template<typename ElmType>
class Array2D {
public:
  typedef std::size_t size_type;
private:
  size_type nRow;
  size_type nCol;
#if __cplusplus >= 201103L
  std::unique_ptr<ElmType[]> data;
#else
  ElmType* data;
#endif

  void
  clone(const Array2D<ElmType>& that) const
  {
    for (size_type i = 0; i < nRow; i++) {
      for (size_type j = 0; j < nCol; j++) {
        (*this)[i][j] = that[i][j];
      }
    }
  }
public:
  Array2D(size_type nRow, size_type nCol) :
    nRow(nRow),
    nCol(nCol),
    data(new ElmType[nRow * nCol])
  {}

  Array2D(const Array2D<ElmType>& that) :
    nRow(that.nRow),
    nCol(that.nCol),
    data(new ElmType[nRow * nCol])
  {
    clone(that);
  }

#if __cplusplus < 201103L
  ~Array2D()
  {
    delete[] data;
  }
#endif

  void
  fill(const ElmType& value) const
  {
    std::fill_n(data, nRow * nCol, value);
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

  ElmType*
  operator[](size_type y) const
  {
    return &data[y * nCol];
  }

  Array2D<ElmType>&
  operator=(const Array2D<ElmType>& that)
  {
    nRow = that.nRow;
    nCol = that.nCol;
    data = new ElmType[nRow * nCol];
    clone(that);
    return *this;
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
};  // class Array2D


#endif  // ARRAY2D_HPP
