#include <queue>
#include <limits>
#include <algorithm>
#include <numeric>
#include "hungarian.h"

namespace hungarian {
  int n, m;
  cost_t cost[MAXN][MAXM]; // [n][m]
  cost_t label_n[MAXN], label_m[MAXM];
  int match_n[MAXN], match_m[MAXM];
  bool intree_n[MAXN], intree_m[MAXM];
  int where_min_cost[MAXM];
  int parent[MAXN];


  //get updated cost of an edge
  inline cost_t updated_cost(int i, int j) {
    return label_n[i] + label_m[j] - cost[i][j];
  }

  inline cost_t get_min_cost(int i) {
    return updated_cost(where_min_cost[i], i);
  }

  void initialize() {
    // Initialize labels
    std::fill(label_m, label_m + m, 0);

    for(int i = 0; i < n; ++i)
      label_n[i] = std::accumulate(cost[i], cost[i] + m, 0, static_cast<const cost_t&(*) (const cost_t&, const cost_t&)>(std::max<cost_t>));

    // Initialize matching
    std::fill(match_n, match_n + n, -1);
    std::fill(match_m, match_m + m, -1);
  }

  void update_labels() {
    cost_t D = std::numeric_limits<cost_t>::max();

    // Find difference
    for(int i = 0; i < m; ++i)
      if(!intree_m[i])
        D = std::min(D, get_min_cost(i));

    // Update labels
    for(int i = 0; i < n; ++i)
      if(intree_n[i])
        label_n[i] -= D;

    for(int i = 0; i < m; ++i)
      if(intree_m[i])
        label_m[i] += D;
  }

  void insert_into_tree(int x, int p) {
    intree_n[x] = true;
    parent[x] = p;

    // Update minimal costes
    for(int i = 0; i < m; ++i)
      if(updated_cost(x, i) < get_min_cost(i))
        where_min_cost[i] = x;
  }

  // Invert edges along an augmenting path
  void finish_step(int x, int y) {
    if(parent[x] != -1)
      finish_step(parent[x], match_n[x]);
    match_n[x] = y;
    match_m[y] = x;
  }

  void step() {
    std::queue<int> Q; // for BFS

    // Initialize arrays
    std::fill(intree_n, intree_n + n, false);
    std::fill(intree_m, intree_m + m, false);
    std::fill(parent, parent + n, -1);

    // Find root
    for(int i = 0; i < n; ++i)
      if(match_n[i] == -1) {
        Q.push(i);
        intree_n[i] = true;
        std::fill(where_min_cost, where_min_cost + m, i);
        break;
      }

    while(true) {
      while(!Q.empty()) {
        int cur = Q.front();
        Q.pop();

        for(int i = 0; i < m; ++i)
          if(!intree_m[i] && updated_cost(cur, i) == 0) {
            if(match_m[i] == -1) {
              finish_step(cur, i);
              return;
            }
            
            intree_m[i] = true;
            insert_into_tree(match_m[i], cur);
            Q.push(match_m[i]);
          }
      }

      update_labels();
      for(int i = 0; i < m; ++i)
        if(!intree_m[i] && get_min_cost(i) == 0) {
          if(match_m[i] == -1) {
            finish_step(where_min_cost[i], i);
            return;
          }
          else {
            intree_m[i] = true;
            if(!intree_n[match_m[i]]) {
              Q.push(match_m[i]);
              insert_into_tree(match_m[i], where_min_cost[i]);
            }
          }
        }
    }
  }

  std::vector<std::pair<int, int> > hungarian(int _n, int _m, cost_t ** _cost) {
    n = _n;
    m = _m;

    // Transform problem into max cost matching
    cost_t MX = 0;
    for(int i = 0; i < n; ++i)
      for(int j = 0; j < m; ++j)
        MX = std::max(MX, _cost[i][j]);

    for(int i = 0; i < n; ++i)
      for(int j = 0; j < m; ++j)
        cost[i][j] = MX - _cost[i][j]; 

    initialize();
    for(int i = 0; i < n && i < m; ++i)
      step();

    std::vector<std::pair<int, int> > V;

    // Generate result
    for(int i = 0; i < n; ++i)
      if(match_n[i] != -1)
        V.push_back(std::make_pair(i, match_n[i]));

    return V;
  }
}

