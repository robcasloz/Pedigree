//
//
//

#ifndef PEDIGREE_ANALYSISSCOPE_HPP
#define PEDIGREE_ANALYSISSCOPE_HPP

#include <cstdint>
// using uint8_t

namespace pedigree {

enum class AnalysisScope : uint8_t {
  // Unknown
  Block,
  Function,
  Interprocedural
};

} // namespace pedigree

#endif // header
