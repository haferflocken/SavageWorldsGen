#pragma once

#include "../hindrance.hpp"

#include <vector>

namespace savage {
class hindrance_manager {
  static std::vector<hindrance>* s_hindrances;
  static std::vector<hindrance>* s_majorHindrances;
  static std::vector<hindrance>* s_minorHindrances;

  hindrance_manager() {}
  ~hindrance_manager() {}

public:
  static const std::vector<hindrance>& get_hindrances();
  static const std::vector<hindrance>& get_major_hindrances();
  static const std::vector<hindrance>& get_minor_hindrances();

  static const hindrance& random_major_hindrance();
  static const hindrance& random_minor_hindrance();

  static void read_hindrances( const std::string& filePath );
};
} // namespace savage
