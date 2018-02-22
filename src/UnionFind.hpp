/*!
 * @brief Implementation of Union Find
 * @author koturn
 * @file UnionFind.hpp
 */
#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include <cstdlib>
#include <numeric>
#include <type_traits>
#include <vector>


/*!
 * @brief Class of Union Find
 * @tparam T  Type of node number (integer)
 */
template<typename T = int>
class UnionFind
{
  static_assert(std::is_integral<T>::value, "[UnionFind] Type of node must be an integer");

public:
  /*!
   * @brief Ctor
   * @param [in] n  Number of node
   */
  UnionFind(T n) noexcept
    : m_par(n)
    , m_rank(n)
  {
    std::iota(std::begin(m_par), std::end(m_par), 0);
  }

  /*!
   * @brief Find node
   * @param [in] x  Node number to find
   * @return node number
   */
  T
  find(T x) noexcept
  {
    return m_par[x] == x ? x : (m_par[x] = find(m_par[x]));
  }

  /*!
   * @brief Merge two groups
   *
   * @param [in] x
   * @param [in] y
   */
  void
  unite(T x, T y) noexcept
  {
    x = find(x);
    y = find(y);
    if (x == y) {
      return;
    }
    if (m_rank[x] < m_rank[y]) {
      m_par[x] = y;
    } else {
      m_par[y] = x;
      if (m_rank[x] == m_rank[y]) {
        m_rank[x]++;
      }
    }
  }

  /*!
   * @brief Identify whether two nodes are same group
   * @param [in] x  First ndde
   * @param [in] y  Second node
   * @return Return true if two nodes are same group, otherwise false
   */
  bool
  isSame(T x, T y) noexcept
  {
    return find(x) == find(y);
  }

private:
  //! Node numbers
  std::vector<T> m_par;
  //! Node ranks
  std::vector<int> m_rank;
};  // class UnionFind


#endif  // UNION_FIND_HPP
