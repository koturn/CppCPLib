/*!
 * @file Graph.hpp
 * @brief Utility classes about graph
 * @author koturn
 */
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <memory>
#include <queue>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>


template<
  typename T,
  typename U
>
struct Edge
{
  Edge(T from_, T to_, U cost_) noexcept
    : from(from_)
    , to(to_)
    , cost(cost_)
  {}

  T from;
  T to;
  U cost;
};  // struct Edge


template<
  typename C,
  typename T = int,
  typename U = int
>
class ShortestPathSolver
{
public:
  /*!
   * @brief Add edge to the graph
   *
   * @param [in] from  Start point of the edge
   * @param [in] to    End point of the edge
   * @param [in] cost  Cost of edge
   */
  void
  addEdge(T from, T to, U cost) noexcept
  {
    static_cast<C*>(this)->addEdge_(from, to, cost);
  }

  void
  addDirectedEdge(T from, T to, U cost) noexcept
  {
    static_cast<C*>(this)->addDirectedEdge_(from, to, cost);
  }

  std::vector<U>
  shortestPath(T from) noexcept
  {
    return static_cast<C*>(this)->shortestPath_(from);
  }

  U
  shortestPath(T from, T to) noexcept
  {
    return shortestPath(from)[to];
  }
};  // class ShortestPathSolver


template<
  typename T = int,
  typename U = int
>
class BellmanFord :
  public ShortestPathSolver<BellmanFord<T, U>, T, U>
{
  friend class ShortestPathSolver<BellmanFord<T, U>, T, U>;

public:
  BellmanFord() noexcept
    : m_graph()
    , m_vertex()
  {}

private:
  void
  addEdge_(T from, T to, U cost) noexcept
  {
    addDirectedEdge_(from, to, cost);
    m_graph.emplace_back(to, from, cost);
  }

  void
  addDirectedEdge_(T from, T to, U cost) noexcept
  {
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph.emplace_back(from, to, cost);
  }

  std::vector<U>
  shortestPath_(T from) const noexcept
  {
    static constexpr int kInf = 0x3f3f3f3f;
    std::vector<U> dists(m_vertex.size());
    std::fill(std::begin(dists), std::end(dists), kInf);
    dists[from] = 0;
    for (;;) {
      auto isUpdated = false;
      for (typename decltype(m_graph)::size_type i = 0; i < m_graph.size(); i++) {
        const auto& e = m_graph[i];
        if (dists[e.from] != kInf && dists[e.to] > dists[e.from] + e.cost) {
          dists[e.to] = dists[e.from] + e.cost;
          isUpdated = true;
        }
      }
      if (!isUpdated) {
        break;
      }
    }
    return dists;
  }

  std::vector<Edge<T, U>> m_graph;
  std::unordered_set<T> m_vertex;
};  // class BellmanFord


template<
  typename T = int,
  typename U = int
>
class Dijkstra :
  public ShortestPathSolver<Dijkstra<T, U>, T, U>
{
  friend class ShortestPathSolver<Dijkstra<T, U>, T, U>;

public:
  Dijkstra() noexcept
    : m_graph()
    , m_vertex()
  {}

  Dijkstra(int v) noexcept
    : m_graph(v)
    , m_vertex()
  {}

private:
  void
  addEdge_(T from, T to, U cost) noexcept
  {
    addDirectedEdge_(from, to, cost);
    m_graph[to].emplace_back(from, to, cost);
  }

  void
  addDirectedEdge_(T from, T to, U cost) noexcept
  {
    auto m = static_cast<typename decltype(m_graph)::size_type>(std::max(from, to));
    if (m_graph.size() <= m) {
      m_graph.resize(m + 1);
    }
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph[from].emplace_back(from, to, cost);
  }

  std::vector<U>
  shortestPath_(T from) const noexcept
  {
    // first is shortest distance，second is vertex node number
    using P = std::pair<U, T>;

    static constexpr int kInf = 0x3f3f3f3f;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pQueue;
    std::vector<U> dists(m_vertex.size());
    std::fill(dists.begin(), dists.end(), kInf);
    dists[from] = 0;
    pQueue.emplace(0, from);
    while (!pQueue.empty()) {
      auto p = pQueue.top();
      pQueue.pop();
      auto v = p.second;
      if (dists[v] < p.first) {
        continue;
      }
      for (typename decltype(m_graph)::size_type i = 0; i < m_graph[v].size(); i++) {
        const auto& e = m_graph[v][i];
        if (dists[e.to] > dists[v] + e.cost) {
          dists[e.to] = dists[v] + e.cost;
          pQueue.emplace(dists[e.to], e.to);
        }
      }
    }
    return dists;
  }

  std::vector<std::vector<Edge<T, U>>> m_graph;
  std::unordered_set<T> m_vertex;
};  // class Dijkstra


template<
  typename T = int,
  typename U = int
>
class WarshalFloyd :
  public ShortestPathSolver<WarshalFloyd<T, U>, T, U>
{
  friend class ShortestPathSolver<WarshalFloyd<T, U>, T, U>;

public:
  WarshalFloyd() noexcept
    : m_nVertex(kDefaultSize)
    , m_graph(new U[kDefaultSize * kDefaultSize])
    , m_vertex()
  {
    std::fill_n(m_graph.get(), kDefaultSize * kDefaultSize, kInf);
    for (decltype(m_nVertex) i = 0; i < m_nVertex; i++) {
      m_graph[i *  m_nVertex + i] = 0;
    }
  }

  WarshalFloyd(std::size_t v) noexcept
    : m_nVertex(v)
    , m_graph(new U[v * v])
    , m_vertex()
  {
    std::fill_n(m_graph.get(), v * v, kInf);
    for (decltype(m_nVertex) i = 0; i < m_nVertex; i++) {
      m_graph[i *  m_nVertex + i] = 0;
    }
  }

private:
  void
  addEdge_(T from, T to, U cost) noexcept
  {
    addDirectedEdge_(from, to, cost);
    m_graph[to * m_nVertex + from] = cost;
  }

  void
  addDirectedEdge_(T from, T to, U cost) noexcept
  {
    using SizeType = decltype(m_nVertex);
    auto m = static_cast<SizeType>(std::max(from, to));

    if (m_nVertex <= m) {
      std::unique_ptr<U[]> origGraph(std::move(m_graph));
      m++;
      m_graph.reset(new U[m * m]);
      for (SizeType i = 0; i < m_nVertex; i++) {
        for (SizeType j = 0; j < m_nVertex; j++) {
          m_graph[i * m + j] = origGraph[i * m_nVertex + j];
        }
      }
      for (SizeType i = 0; i < m_nVertex; i++) {
        for (SizeType j = m_nVertex; j < m; j++) {
          m_graph[i * m + j] = kInf;
        }
      }
      for (SizeType i = m_nVertex; i < m; i++) {
        for (SizeType j = 0; j < m; j++) {
          m_graph[i * m + j] = kInf;
        }
      }
      for (SizeType i = 0; i < m; i++) {
        m_graph[i * m + i] = 0;
      }
    }
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph[from * m_nVertex + to] = cost;
  }

  std::vector<U>
  shortestPath_(T from) noexcept
  {
    using SizeType = decltype(m_nVertex);
    for (SizeType k = 0; k < m_nVertex; k++) {
      for (SizeType i = 0; i < m_nVertex; i++) {
        for (SizeType j = 0; j < m_nVertex; j++) {
          m_graph[i * m_nVertex + j] = std::min(m_graph[i * m_nVertex + j], m_graph[i * m_nVertex + k] + m_graph[k * m_nVertex + j]);
        }
      }
    }
    auto ptr = m_graph.get() + from * m_nVertex;
    return std::vector<U>(ptr, ptr + m_nVertex);
  }

  static constexpr std::size_t kDefaultSize = 16;
  static constexpr int kInf = 0x3f3f3f3f;
  std::size_t m_nVertex;
  std::unique_ptr<U[]> m_graph;
  std::unordered_set<T> m_vertex;
};  // class WarshalFloyd


#endif  // GRAPH_HPP
