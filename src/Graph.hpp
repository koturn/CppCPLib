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


template<
  typename C,
  typename T = int,
  typename U = int
>
class SpanningTree
{
private:
  using E = Edge<T, U>;

public:
  SpanningTree(std::size_t graphSize) noexcept
    : m_graph(graphSize)
  {}

  void
  addEdge(T from, T to, U cost) noexcept
  {
    m_graph[from].emplace_back(from, to, cost);
  }

  void
  addEdge(const E& edge) noexcept
  {
    m_graph[edge.from].push_back(edge);
  }

  std::pair<U, std::vector<E>>
  solve() noexcept
  {
    return static_cast<C*>(this)->solve_();
  }

protected:
  std::vector<std::vector<E>>&
  getGraphRef() noexcept
  {
    return m_graph;
  }

private:
  std::vector<std::vector<E>> m_graph;
};  // class SpanningTree


template<
  typename T = int,
  typename U = int
>
class SpanningTreePrim
  : public SpanningTree<SpanningTreePrim<T, U>, T, U>
{
private:
  using Base = SpanningTree<SpanningTreePrim<T, U>, T, U>;
  using E = Edge<T, U>;

  friend Base;

public:
  SpanningTreePrim(std::size_t graphSize) noexcept
    : Base(graphSize)
  {}

private:
  std::pair<U, std::vector<E>>
  solve_(T r = 0) noexcept
  {
    std::vector<E> edges;
    U total = 0;
    auto& graph = Base::getGraphRef();
    std::vector<bool> isVisited(graph.size());

    const auto compare = [](const E& x, const E& y) {
      return x.cost > y.cost;
    };
    std::priority_queue<E, std::vector<E>, decltype(compare)> q(compare);
    q.emplace(-1, r, 0);

    while (!q.empty()) {
      const auto e = q.top();
      q.pop();
      if (isVisited[e.to]) {
        continue;
      }
      isVisited[e.to] = true;
      total += e.cost;
      if(e.from != -1) {
        edges.push_back(e);
      }
      for (const auto& f : graph[e.to]) {
        if (!isVisited[f.to]) {
          q.push(f);
        }
      }
    }
    return std::make_pair(total, edges);
  }
};  // class SpanningTreePrim


template<
  typename T = int,
  typename U = int
>
class SpanningTreeKruskal
  : public SpanningTree<SpanningTreeKruskal<T, U>, T, U>
{
private:
  using Base = SpanningTree<SpanningTreeKruskal<T, U>, T, U>;
  using E = Edge<T, U>;

  friend Base;

public:
  SpanningTreeKruskal(std::size_t graphSize) noexcept
    : Base(graphSize)
  {}

private:
  std::pair<U, std::vector<E>>
  solve_() noexcept
  {
    auto& graph = Base::getGraphRef();

    UnionFind<T> uf(graph.size());

    std::vector<E> allEdges;
    for (const auto& adj : graph) {
      std::copy(std::begin(adj), std::end(adj), std::back_inserter(allEdges));
    }

    std::sort(
      std::begin(allEdges),
      std::end(allEdges),
      [](const E& x, const E& y) {
        return x.cost < y.cost;
      });

    U total = 0;
    std::vector<E> edges;
    for (const auto& e : allEdges) {
      if (!uf.isSame(e.from, e.to)) {
        edges.push_back(e);
        total += e.cost;
        uf.unite(e.from, e.to);
      }
    }
    return std::make_pair(total, edges);
  }
};  // class SpanningTreeKruskal


#endif  // GRAPH_HPP
