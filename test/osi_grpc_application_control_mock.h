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

class ApplicationControlClientMock {
 public:
  ApplicationControlClientMock(std::shared_ptr<grpc::Channel> channel)
      : stub_(osi3::grpc::ApplicationControl::NewStub(channel)) {}

  auto Initialize() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::ApplicationControl_Meta{};
    auto response = osi3::ApplicationControl_State{};

    auto status = stub_->Initialize(&context, request, &response);
    return status.ok();
  }

  auto Start() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::ApplicationControl_Meta{};
    auto response = osi3::ApplicationControl_State{};

    auto status = stub_->Start(&context, request, &response);
    return status.ok();
  }

  auto Pause() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::ApplicationControl_Meta{};
    auto response = osi3::ApplicationControl_State{};

    auto status = stub_->Pause(&context, request, &response);
    return status.ok();
  }

  auto Stop() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::ApplicationControl_Meta{};
    auto response = osi3::ApplicationControl_State{};

    auto status = stub_->Stop(&context, request, &response);
    return status.ok();
  }

  auto Log() -> bool {
    auto context = grpc::ClientContext{};
    auto request = osi3::ApplicationControl_Log{};
    auto response = google::protobuf::Empty{};

    auto status = stub_->Log(&context, request, &response);
    return status.ok();
  }

 private:
  std::unique_ptr<osi3::grpc::ApplicationControl::Stub> stub_;
};

class ApplicationControlServerMock final : public osi3::grpc::ApplicationControl::Service {
 public:
  MOCK_METHOD(grpc::Status, Initialize,
              (grpc::ServerContext * context, const osi3::ApplicationControl_Meta *request,
               osi3::ApplicationControl_State *response),
              (override));
  MOCK_METHOD(grpc::Status, Start,
              (grpc::ServerContext * context, const osi3::ApplicationControl_Meta *request,
               osi3::ApplicationControl_State *response),
              (override));
  MOCK_METHOD(grpc::Status, Pause,
              (grpc::ServerContext * context, const osi3::ApplicationControl_Meta *request,
               osi3::ApplicationControl_State *response),
              (override));
  MOCK_METHOD(grpc::Status, Stop,
              (grpc::ServerContext * context, const osi3::ApplicationControl_Meta *request,
               osi3::ApplicationControl_State *response),
              (override));
  MOCK_METHOD(grpc::Status, Log,
              (grpc::ServerContext * context, const osi3::ApplicationControl_Log *request,
               google::protobuf::Empty *response),
              (override));
};
