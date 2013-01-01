#ifndef _HUNGARIAN_H
#define _HUNGARIAN_H

#include <utility>
#include <vector>

namespace hungarian {
  const int MAXN = 500;
  const int MAXM = 500;
  typedef int cost_t;

  cost_t updated_cost(int, int);
  cost_t get_min_cost(int);
  void initialize();
  void update_labels();
  void insert_into_tree(int, int);
  void finish_step(int, int);
  void step();

  // arguments: n, m, cost[n][m]
  std::vector<std::pair<int, int> > hungarian(int, int, cost_t **);
}

#endif // _HUNGARIAN_H
