//
//
//

#ifndef PEDIGREE_PDGRAPH_HPP
#define PEDIGREE_PDGRAPH_HPP

#include "Config.hpp"

#include "Analysis/Graphs/DependenceGraphs.hpp"

namespace pedigree {

using ProgramDependenceNode = InstructionDependenceNode;
using PDGraph = InstructionDependenceGraph;

} // namespace pedigree end

#endif // header