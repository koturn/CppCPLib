/*!
 * @file Graph.hpp
 * @brief Utility classes about graph
 * @author koturn
 */
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <type_traits>
#include <unordered_set>
#include <vector>


template<typename T>
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
  addEdge(int from, int to, int cost) noexcept
  {
    static_cast<T*>(this)->addEdge_(from, to, cost);
  }

  void
  addDirectedEdge(int from, int to, int cost) noexcept
  {
    static_cast<T*>(this)->addDirectedEdge_(from, to, cost);
  }

  std::vector<int>
  shortestPath(int from) noexcept
  {
    return static_cast<T*>(this)->shortestPath_(from);
  }

  int
  shortestPath(int from, int to) noexcept
  {
    return shortestPath(from)[to];
  }
};  // class ShortestPathSolver


class BellmanFord :
  public ShortestPathSolver<BellmanFord>
{
  friend class ShortestPathSolver<BellmanFord>;

private:
  struct Edge {
    Edge() :
      from(0),
      to(0),
      cost(0)
    {}

    Edge(int from_, int to_, int cost_) :
      from(from_),
      to(to_),
      cost(cost_)
    {}

    int from;
    int to;
    int cost;
  };

public:
  BellmanFord() :
    ShortestPathSolver(),
    m_graph(),
    m_vertex()
  {}

private:
  void
  addEdge_(int from, int to, int cost) noexcept
  {
    addDirectedEdge(from, to, cost);
    m_graph.emplace_back(to, from, cost);
  }

  void
  addDirectedEdge_(int from, int to, int cost) noexcept
  {
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph.emplace_back(from, to, cost);
  }

  std::vector<int>
  shortestPath_(int from) const noexcept
  {
    static constexpr int kInf = 0x3f3f3f3f;
    std::vector<int> dists(m_vertex.size());
    std::fill(std::begin(dists), std::end(dists), kInf);
    dists[from] = 0;
    for (;;) {
      auto isUpdated = false;
      for (decltype(m_graph)::size_type i = 0; i < m_graph.size(); i++) {
        const Edge& e = m_graph[i];
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

  std::vector<Edge> m_graph;
  std::unordered_set<int> m_vertex;
};  // class BellmanFord


class Dijkstra :
  public ShortestPathSolver<Dijkstra>
{
  friend class ShortestPathSolver<Dijkstra>;

private:
  struct Edge {
    int to;
    int cost;
    Edge() :
      to(0),
      cost(0)
    {}

    Edge(int to_, int cost_) :
      to(to_),
      cost(cost_)
    {}
  };

public:
  Dijkstra() :
    ShortestPathSolver(),
    m_graph(),
    m_vertex()
  {}

  Dijkstra(int v) :
    ShortestPathSolver(),
    m_graph(v),
    m_vertex()
  {}

private:
  void
  addEdge_(int from, int to, int cost)
  {
    addDirectedEdge(from, to, cost);
    m_graph[to].emplace_back(Edge(from, cost));
  }

  void
  addDirectedEdge_(int from, int to, int cost)
  {
    auto m = static_cast<decltype(m_graph)::size_type>(std::max(from, to));
    if (m_graph.size() <= m) {
      m_graph.resize(m + 1);
    }
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph[from].emplace_back(Edge(to, cost));
  }

  std::vector<int>
  shortestPath_(int from) const
  {
    using P = std::pair<int, int>;  // firstは最短距離，secondは頂点の番号
    static constexpr int kInf = 0x3f3f3f3f;
    std::priority_queue<P, std::vector<P>, std::greater<P> > pQueue;
    std::vector<int> dists(m_vertex.size());
    std::fill(dists.begin(), dists.end(), kInf);
    dists[from] = 0;
    pQueue.push(P(0, from));
    while (!pQueue.empty()) {
      auto p = pQueue.top();
      pQueue.pop();
      auto v = p.second;
      if (dists[v] < p.first) {
        continue;
      }
      for (decltype(m_graph)::size_type i = 0; i < m_graph[v].size(); i++) {
        const auto& e = m_graph[v][i];
        if (dists[e.to] > dists[v] + e.cost) {
          dists[e.to] = dists[v] + e.cost;
          pQueue.push(P(dists[e.to], e.to));
        }
      }
    }
    return dists;
  }

  std::vector<std::vector<Edge> > m_graph;
  std::unordered_set<int> m_vertex;
};  // class Dijkstra


class WarshalFloyd :
  public ShortestPathSolver<WarshalFloyd>
{
  friend class ShortestPathSolver<WarshalFloyd>;

public:
  WarshalFloyd() :
    ShortestPathSolver(),
    nVertex(kDefaultSize),
    m_graph(new int[kDefaultSize * kDefaultSize]),
    m_vertex()
  {
    std::fill_n(m_graph.get(), kDefaultSize * kDefaultSize, kInf);
    for (int i = 0; i < nVertex; i++) {
      m_graph[i *  nVertex + i] = 0;
    }
  }

  WarshalFloyd(int v) :
    nVertex(v),
    m_graph(new int[v * v]),
    m_vertex()
  {
    std::fill_n(m_graph.get(), v * v, kInf);
    for (int i = 0; i < nVertex; i++) {
      m_graph[i *  nVertex + i] = 0;
    }
  }

private:
  void
  addEdge_(int from, int to, int cost) noexcept
  {
    addDirectedEdge(from, to, cost);
    m_graph[to * nVertex + from] = cost;
  }

  void
  addDirectedEdge_(int from, int to, int cost) noexcept
  {
    int m = std::max(from, to);
    if (nVertex <= m) {
      std::unique_ptr<int[]> origGraph(std::move(m_graph));
      m++;
      m_graph.reset(new int[m * m]);
      for (int i = 0; i < nVertex; i++) {
        for (int j = 0; j < nVertex; j++) {
          m_graph[i * m + j] = origGraph[i * nVertex + j];
        }
      }
      for (int i = 0; i < nVertex; i++) {
        for (int j = nVertex; j < m; j++) {
          m_graph[i * m + j] = kInf;
        }
      }
      for (int i = nVertex; i < m; i++) {
        for (int j = 0; j < m; j++) {
          m_graph[i * m + j] = kInf;
        }
      }
      for (int i = 0; i < m; i++) {
        m_graph[i * m + i] = 0;
      }
    }
    m_vertex.insert(from);
    m_vertex.insert(to);
    m_graph[from * nVertex + to] = cost;
  }

  std::vector<int>
  shortestPath_(int from) noexcept
  {
    for (int k = 0; k < nVertex; k++) {
      for (int i = 0; i < nVertex; i++) {
        for (int j = 0; j < nVertex; j++) {
          m_graph[i * nVertex + j] = std::min(m_graph[i * nVertex + j], m_graph[i * nVertex + k] + m_graph[k * nVertex + j]);
        }
      }
    }
    auto ptr = m_graph.get() + from * nVertex;
    return std::vector<int>(ptr, ptr + nVertex);
  }

  static constexpr int kDefaultSize = 16;
  static constexpr int kInf = 0x3f3f3f3f;
  int nVertex;
  std::unique_ptr<int[]> m_graph;
  std::unordered_set<int> m_vertex;
};  // class WarshalFloyd


#endif  // GRAPH_HPP
