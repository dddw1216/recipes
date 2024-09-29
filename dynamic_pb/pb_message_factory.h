// Copyright (c) 2024, dddw1216. All rights reserved.

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/message.h"

namespace recipes::dynamic_pb {

struct PbMessageSource {
  std::string dir_name;
  std::string file_name;
  std::string msg_name;
  std::string msg_description;

  const std::string GetKey() const { return dir_name + file_name; }

  const std::string ToString() const {
    std::stringstream ss;
    ss << "dir_name=" << dir_name << ",file_name=" << file_name << ",msg_name=" << msg_name;
    ss << ",msg_description:\n" << msg_description;
    return ss.str();
  }
};

class MultiFileErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector {
  virtual void AddError(const std::string& filename, int line, int column, const std::string& message) override {
    std::cout << "file=" << filename << ", line=" << line << ", column=" << column << ", message=" << message
              << std::endl;
  }
};

struct PbMessageDynamic {
  MultiFileErrorCollector* p_error_collector = nullptr;
  google::protobuf::compiler::SourceTree* p_source_tree = nullptr;
  google::protobuf::compiler::Importer* p_importer = nullptr;
  const google::protobuf::Descriptor* p_descriptor = nullptr;
};

class PbMessageFactory {
 public:
  PbMessageFactory() = default;
  ~PbMessageFactory() = default;

  static PbMessageFactory& GetInstance() {
    static PbMessageFactory ins;
    return ins;
  }

 public:
  bool AddProtoFile(const PbMessageSource& source) {
    PbMessageDynamic pb_message_dynamic;
    pb_message_dynamic.p_error_collector = new MultiFileErrorCollector();
    pb_message_dynamic.p_source_tree = new google::protobuf::compiler::DiskSourceTree();
    dynamic_cast<google::protobuf::compiler::DiskSourceTree*>(pb_message_dynamic.p_source_tree)
        ->MapPath("", source.dir_name);
    pb_message_dynamic.p_importer = new google::protobuf::compiler::Importer(pb_message_dynamic.p_source_tree,
                                                                             pb_message_dynamic.p_error_collector);
    const google::protobuf::FileDescriptor* p_file_des = pb_message_dynamic.p_importer->Import(source.file_name);
    if (p_file_des == nullptr) {
      std::cout << "[ERROR] p_file_des is nullptr" << std::endl;
      return false;
    }
    /// Print file_descriptor
    std::cout << "[INFO] p_file_des debug_string:\n" << p_file_des->DebugString() << std::endl;
    for (int i = 0; i < p_file_des->dependency_count(); ++i) {
      std::cout << "[INFO] p_file_des dependency i=" << i << ", debug_string:\n"
                << p_file_des->dependency(i)->DebugString() << std::endl;
    }
    pb_message_dynamic.p_descriptor =
        pb_message_dynamic.p_importer->pool()->FindMessageTypeByName(source.msg_name);
    if (pb_message_dynamic.p_descriptor == nullptr) {
      std::cout << "[ERROR] p_descriptor is nullptr" << std::endl;
      return false;
    }
    /// Print message_descriptor
    std::cout << "[INFO] p_descriptor debug_string:\n" << pb_message_dynamic.p_descriptor->DebugString() << std::endl;

    /// Add to map and print succ
    dynamic_map_[source.GetKey()] = pb_message_dynamic;
    std::cout << "[INFO] succ add to dynamic_map with key=" << source.GetKey() << std::endl;
    return true;
  }

  google::protobuf::Message* NewMessage(const PbMessageSource& source) {
    auto it = dynamic_map_.find(source.GetKey());
    if (it == dynamic_map_.end()) {
      std::cout << "[ERROR] can't find with source: " << source.ToString() << std::endl;
      return nullptr;
    }
    const PbMessageDynamic& pb_message_dynamic = it->second;
    const google::protobuf::Message* p_msg = pb_dynamic_factory_.GetPrototype(pb_message_dynamic.p_descriptor);
    if (p_msg == nullptr) {
      std::cout << "[ERROR] p_msg is nullptr" << std::endl;
      return nullptr;
    }
    return p_msg->New();
  }

 public:
  google::protobuf::DynamicMessageFactory pb_dynamic_factory_;
  std::unordered_map<std::string, PbMessageDynamic> dynamic_map_;

 private:
  PbMessageFactory(const PbMessageFactory&) = delete;
  PbMessageFactory& operator=(const PbMessageFactory&) = delete;
  PbMessageFactory(PbMessageFactory&&) = delete;
  PbMessageFactory& operator=(PbMessageFactory&&) = delete;
};

}  // namespace recipes::dynamic_pb
