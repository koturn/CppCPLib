#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <type_traits>
#include <memory>


class ShortestPathSolver
{
public:
  virtual inline void addEdge(int from, int to, int cost) = 0;
  virtual inline void addDirectedEdge(int from, int to, int cost) = 0;
  virtual inline std::vector<int> shortestPath(int from) = 0;

  inline int shortestPath(int from, int to)
  {
    return shortestPath(from)[to];
  }
  virtual ~ShortestPathSolver() = default;
};  // class ShortestPathSolver


class BellmanFord :
  public ShortestPathSolver
{
private:
  struct Edge {
    int from;
    int to;
    int cost;
    Edge() :
      from(0),
      to(0),
      cost(0)
    {}
    Edge(int from, int to, int cost) :
      from(from),
      to(to),
      cost(cost)
    {}
  };
  std::vector<Edge> graph;
  std::unordered_set<int> vertex;
public:
  BellmanFord() :
    ShortestPathSolver(),
    graph(),
    vertex()
  {}

  void
  addEdge(int from, int to, int cost)
  {
    addDirectedEdge(from, to, cost);
    graph.emplace_back(Edge(to, from, cost));
  }

  void
  addDirectedEdge(int from, int to, int cost)
  {
    vertex.insert(from);
    vertex.insert(to);
    graph.emplace_back(Edge(from, to, cost));
  }

  std::vector<int>
  shortestPath(int from)
  {
    static constexpr int INF = 0x3f3f3f3f;
    std::vector<int> dists(vertex.size());
    std::fill(dists.begin(), dists.end(), INF);
    dists[from] = 0;
    while (true) {
      bool isUpdated = false;
      for (decltype(graph)::size_type i = 0; i < graph.size(); i++) {
        const Edge& e = graph[i];
        if (dists[e.from] != INF && dists[e.to] > dists[e.from] + e.cost) {
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
};  // class BellmanFord


class Dijkstra :
  public ShortestPathSolver
{
private:
  struct Edge {
    int to;
    int cost;
    Edge() :
      to(0),
      cost(0)
    {}
    Edge(int to, int cost) :
      to(to),
      cost(cost)
    {}
  };

  std::vector<std::vector<Edge> > graph;
  std::unordered_set<int> vertex;
public:
  Dijkstra() :
    ShortestPathSolver(),
    graph(),
    vertex()
  {}

  Dijkstra(int v) :
    ShortestPathSolver(),
    graph(v),
    vertex()
  {}

  void
  addEdge(int from, int to, int cost)
  {
    addDirectedEdge(from, to, cost);
    graph[to].emplace_back(Edge(from, cost));
  }

  void
  addDirectedEdge(int from, int to, int cost)
  {
    int m = std::max(from, to);
    if (graph.size() <= m) {
      graph.resize(m + 1);
    }
    vertex.insert(from);
    vertex.insert(to);
    graph[from].emplace_back(Edge(to, cost));
  }

  std::vector<int>
  shortestPath(int from)
  {
    using P = std::pair<int, int>;  // firstは最短距離，secondは頂点の番号
    static constexpr int INF = 0x3f3f3f3f;
    std::priority_queue<P, std::vector<P>, std::greater<P> > pQueue;
    std::vector<int> dists(vertex.size());
    std::fill(dists.begin(), dists.end(), INF);
    dists[from] = 0;
    pQueue.push(P(0, from));
    while (!pQueue.empty()) {
      P p = pQueue.top(); pQueue.pop();
      int v = p.second;
      if (dists[v] < p.first) {
        continue;
      }
      for (decltype(graph)::size_type i = 0; i < graph[v].size(); i++) {
        const Edge& e = graph[v][i];
        if (dists[e.to] > dists[v] + e.cost) {
          dists[e.to] = dists[v] + e.cost;
          pQueue.push(P(dists[e.to], e.to));
        }
      }
    }
    return dists;
  }
};  // class Dijkstra


class WarshalFloyd :
  public ShortestPathSolver
{
private:
  static constexpr int DEFAULT_SIZE = 16;
  static constexpr int INF = 0x3f3f3f3f;
  int nVertex;
  std::unique_ptr<int[]> graph;
  std::unordered_set<int> vertex;
public:
  WarshalFloyd() :
    ShortestPathSolver(),
    nVertex(DEFAULT_SIZE),
    graph(new int[DEFAULT_SIZE * DEFAULT_SIZE]),
    vertex()
  {
    std::fill_n(graph.get(), DEFAULT_SIZE * DEFAULT_SIZE, INF);
    for (int i = 0; i < nVertex; i++) {
      graph[i *  nVertex + i] = 0;
    }
  }


  WarshalFloyd(int v) :
    nVertex(v),
    graph(new int[v * v]),
    vertex()
  {
    std::fill_n(graph.get(), v * v, INF);
    for (int i = 0; i < nVertex; i++) {
      graph[i *  nVertex + i] = 0;
    }
  }


  void
  addEdge(int from, int to, int cost)
  {
    addDirectedEdge(from, to, cost);
    graph[to * nVertex + from] = cost;
  }

  void
  addDirectedEdge(int from, int to, int cost)
  {
    int m = std::max(from, to);
    if (nVertex <= m) {
      std::unique_ptr<int[]> origGraph(std::move(graph));
      m++;
      graph.reset(new int[m * m]);
      for (int i = 0; i < nVertex; i++) {
        for (int j = 0; j < nVertex; j++) {
          graph[i * m + j] = origGraph[i * nVertex + j];
        }
      }
      for (int i = 0; i < nVertex; i++) {
        for (int j = nVertex; j < m; j++) {
          graph[i * m + j] = INF;
        }
      }
      for (int i = nVertex; i < m; i++) {
        for (int j = 0; j < m; j++) {
          graph[i * m + j] = INF;
        }
      }
      for (int i = 0; i < m; i++) {
        graph[i * m + i] = 0;
      }
    }
    vertex.insert(from);
    vertex.insert(to);
    graph[from * nVertex + to] = cost;
  }

  std::vector<int>
  shortestPath(int from)
  {
    for (int k = 0; k < nVertex; k++) {
      for (int i = 0; i < nVertex; i++) {
        for (int j = 0; j < nVertex; j++) {
          graph[i * nVertex + j] = std::min(graph[i * nVertex + j], graph[i * nVertex + k] + graph[k * nVertex + j]);
        }
      }
    }
    auto ptr = graph.get() + from * nVertex;
    return std::vector<int>(ptr, ptr + nVertex);
  }
};  // class WarshalFloyd


#endif  // GRAPH_HPP
