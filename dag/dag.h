#pragma once

#include <cstdint>
#include <map>
#include <functional>
#include <initializer_list>
#include <atomic>

#include "dag_node.h"
#include "dag_context.h"
#include "common/singleton.h"
#include "common/common.h"
#include "common/concurrent_blocking_queue.h"

namespace minerva {

class Dag {
  friend class DagEngine;
 public:
  Dag();
  ~Dag();
  DataNode* NewDataNode(const DataNodeMeta& meta, const DataNodeContext& ctx);
  OpNode* NewOpNode(std::initializer_list<DataNode*> inputs,
      std::initializer_list<DataNode*> outputs,
      const OpNode::Runner& runner, const OpNodeContext& ctx);

 private:
  DISALLOW_COPY_AND_ASSIGN(Dag);
  static uint64_t index_counter_;
  std::map<uint64_t, DagNode*> index_to_node_;
};

} // end of namespace minerva
