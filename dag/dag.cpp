#include "dag.h"
#include "dag_node.h"
#include "common/concurrent_blocking_queue.h"
#include <cstdint>
#include <functional>
#include <queue>
#include <cstdio>
#include <thread>
#include <chrono>

using namespace std;

namespace minerva {

uint64_t Dag::index_counter_ = 0;

Dag::Dag() {
}

Dag::~Dag() {
  for (auto i: index_to_node_) {
    delete i.second;
  }
}

DataNode* Dag::NewDataNode(const DataNodeMeta& meta, const DataNodeContext& ctx) {
  DataNode* ret = new DataNode;
  ret->set_meta(meta);
  ret->set_context(ctx);
  ret->node_id_ = index_counter_++;
  index_to_node_.insert(pair<uint64_t, DagNode*>(ret->node_id_, ret));
  return ret;
}

OpNode* Dag::NewOpNode(std::initializer_list<DataNode*> inputs,
    std::initializer_list<DataNode*> outputs,
    const OpNode::Runner& runner, const OpNodeContext& ctx) {
  OpNode* ret = new OpNode;
  ret->set_runner(runner);
  ret->set_context(ctx);
  ret->node_id_ = index_counter_++;
  index_to_node_.insert(pair<uint64_t, DagNode*>(ret->node_id_, ret));
  for(auto in : inputs) {
    ret->AddParent(in);
  }
  for(auto out : outputs) {
    out->AddParent(ret);
  }
  return ret;
}

} // end of namespace minerva
