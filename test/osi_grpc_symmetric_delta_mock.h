//------------------------------------------------------------------------------
// Copyright (c) 2023, MBition GmbH
//
// This program and the accompanying materials are made
// available under the terms of the Mozilla Public License Version 2.0
// which is available at https://www.mozilla.org/en-US/MPL/2.0/
//
// SPDX-License-Identifier: MPL-2.0
//------------------------------------------------------------------------------

#include <gmock/gmock.h>
#include <google/protobuf/empty.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/server_context.h>
#include <gtest/gtest.h>

#include "osi_application_control.pb.h"
#include "osi_grpc_application_control.grpc.pb.h"
#include "osi_grpc_symmetric_delta.grpc.pb.h"
#include "osi_streaming.pb.h"

class SymmetricDeltaClientMock {
 public:
  SymmetricDeltaClientMock(std::shared_ptr<grpc::Channel> channel)
      : stub_(osi3::grpc::SymmetricDelta::NewStub(channel)) {}

  auto SymmetricDelta() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::Streaming{};
    auto response = google::protobuf::Empty{};

    auto status = stub_->SymmetricDelta(&context, request, &response);
    return status.ok();
  }

 private:
  std::unique_ptr<osi3::grpc::SymmetricDelta::Stub> stub_;
};

class SymmetricDeltaServerMock final : public osi3::grpc::SymmetricDelta::Service {
 public:
  MOCK_METHOD(grpc::Status, SymmetricDelta,
              (grpc::ServerContext * context, const osi3::Streaming *request, google::protobuf::Empty *response),
              (override));
};
