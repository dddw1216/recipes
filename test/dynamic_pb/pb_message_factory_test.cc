// Copyright (c) 2024, dddw1216. All rights reserved.

#include <cassert>
#include <iostream>
#include <string>

#include "dynamic_pb/pb_message_factory.h"
#include "dynamic_pb/proto/item_node.pb.h"

using namespace recipes::dynamic_pb;

int main(int argc, char** argv) {
  ItemNode static_item_node;
  static_item_node.set_aspect_ratio(1.234);
  std::cout << "static_item_node:\n" << static_item_node.DebugString() << std::endl;

  PbMessageSource pb_message_source;
  pb_message_source.dir_name = "/data/project/dddw1216/recipes/dynamic_pb/proto/";
  pb_message_source.file_name = "item_node.proto";
  pb_message_source.msg_name = "recipes.dynamic_pb.ItemNode";
  assert(PbMessageFactory::GetInstance().AddProtoFile(pb_message_source));

  google::protobuf::Message* p_dynamic_msg = PbMessageFactory::GetInstance().NewMessage(pb_message_source);
  const google::protobuf::Descriptor* p_des = p_dynamic_msg->GetDescriptor();
  const google::protobuf::Reflection* p_ref = p_dynamic_msg->GetReflection();
  for (int i = 0; i < p_des->field_count(); i++) {
    const google::protobuf::FieldDescriptor* p_field_des = p_des->field(i);
    /// Only set single
    if (p_field_des->label() != google::protobuf::FieldDescriptor::LABEL_OPTIONAL) {
      continue;
    }
    if (p_field_des->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_STRING) {
      p_ref->SetString(p_dynamic_msg, p_field_des, "tag_" + std::to_string(i + 1) + "_field_string");
    }
    if (p_field_des->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT64) {
      p_ref->SetInt64(p_dynamic_msg, p_field_des, 10000 + i + 1);
    }
    if (p_field_des->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_FLOAT) {
      p_ref->SetFloat(p_dynamic_msg, p_field_des, 0.123 + i + 1);
    }
    if (p_field_des->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE) {
      p_ref->SetDouble(p_dynamic_msg, p_field_des, 0.456 + i + 1);
    }
  }
  
  /// Set unknow_field
  int start_unknown_field_idx = p_des->field_count();
  p_ref->MutableUnknownFields(p_dynamic_msg)->AddLengthDelimited(start_unknown_field_idx, "unknown_field_" + std::to_string(start_unknown_field_idx));
  start_unknown_field_idx++;
  p_ref->MutableUnknownFields(p_dynamic_msg)->AddVarint(start_unknown_field_idx, 20000 + start_unknown_field_idx);
  start_unknown_field_idx++;
  p_ref->MutableUnknownFields(p_dynamic_msg)->AddFixed32(start_unknown_field_idx, 20000 + start_unknown_field_idx);
  std::cout << "p_dynamic_msg debug_string:\n" << p_dynamic_msg->DebugString() << std::endl;

  /// Decode from p_dynamic_msg with unknown fields
  std::string pb_bin = p_dynamic_msg->SerializeAsString();
  std::cout << "pb_bin_size=" << pb_bin.size() << std::endl;
  google::protobuf::Message* p_copy_msg = PbMessageFactory::GetInstance().NewMessage(pb_message_source);
  assert(p_copy_msg->ParseFromString(pb_bin));
  /// Get unknown fields
  {
    const google::protobuf::Reflection* p_ref = p_copy_msg->GetReflection();
    for (int i = 0; i < p_ref->GetUnknownFields(*p_copy_msg).field_count(); i++) {
      const google::protobuf::UnknownField& unknonwn_field = p_ref->GetUnknownFields(*p_copy_msg).field(i);
      std::cout << "i=" << i << ",number=" << unknonwn_field.number() << ",type=" << unknonwn_field.type() << ",";
      google::protobuf::UnknownField::Type type = unknonwn_field.type();
      if (type == google::protobuf::UnknownField::TYPE_VARINT) {
        std::cout << "value(varint)=" << unknonwn_field.varint() << std::endl;
      } else if (type == google::protobuf::UnknownField::TYPE_FIXED32) {
        std::cout << "value(fixed32)=" << unknonwn_field.fixed32() << std::endl;
      } else if (type == google::protobuf::UnknownField::TYPE_FIXED64) {
        std::cout << "value(fixed64)=" << unknonwn_field.fixed64() << std::endl;
      } else if (type == google::protobuf::UnknownField::TYPE_LENGTH_DELIMITED) {
        std::cout << "value(length_delimited)=" << unknonwn_field.length_delimited() << std::endl;
      } else {
        std::cout << "unsupport_type=" << type << std::endl;
      }
    }
  }

  return 0;
}
