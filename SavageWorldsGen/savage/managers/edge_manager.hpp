#pragma once

#include "../edge.hpp"
#include "../person.hpp"

namespace savage {
class edge_manager {
  static std::vector<edge>* s_edges;

  edge_manager() {}
  ~edge_manager() {}

public:
  static const std::vector<edge>& get_edges();

  static std::vector<const edge*> allowed_edges( const person& p );
  static const edge* random_allowed_edge( const person& p );

  static void read_edges( const std::string& filePath );
};
} // namespace savage
