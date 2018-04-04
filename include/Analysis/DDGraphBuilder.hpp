//
//
//

#ifndef PEDIGREE_DDGRAPHBUILDER_HPP
#define PEDIGREE_DDGRAPHBUILDER_HPP

#include "Config.hpp"

#include "DDGraph.hpp"

#include "llvm/IR/Instruction.h"
// using llvm::Instruction

#include "llvm/IR/InstVisitor.h"
// using llvm::InstVisitor

namespace pedigree {

class DDGraphBuilder : public llvm::InstVisitor<DDGraphBuilder> {
  DDGraph &Graph;

public:
  DDGraphBuilder(DDGraph &Graph) : Graph(Graph) {}

  template <typename T> void build(T &Unit) { visit(Unit); }

  void visitInstruction(llvm::Instruction &CurInstruction) {
    BasicDependenceInfo info{};
    info.origins |= DependenceOrigin::Data;
    // always flow for SSA use-def chains
    // info.setHazard(DependenceHazard::flow);

    auto src = Graph.getOrInsertNode(&CurInstruction);
    for (auto &u : CurInstruction.uses()) {
      auto *user = llvm::dyn_cast<llvm::Instruction>(u.getUser());
      if (user) {
        auto dst = Graph.getOrInsertNode(user);
        src->addDependentNode(dst, info);
      }
    }
  }
};

} // namespace pedigree end

#endif // header
