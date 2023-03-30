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
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/server_builder_option.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>

#include "test/osi_grpc_symmetric_delta_mock.h"

namespace {

class OsiGrpcSymmetricDeltaTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto builder = grpc::ServerBuilder{};
    builder.RegisterService(&service_);
    server_ = builder.BuildAndStart();

    auto channel_args = grpc::ChannelArguments{};
    auto channel = server_->InProcessChannel(channel_args);

    server_thread_ = std::make_unique<std::thread>([this]() { server_->Wait(); });
    client_ = std::make_unique<SymmetricDeltaClientMock>(channel);
  }

  void TearDown() override {
    server_->Shutdown();
    server_thread_->join();
  }

  std::unique_ptr<grpc::Server> server_;
  std::unique_ptr<std::thread> server_thread_;
  std::unique_ptr<SymmetricDeltaClientMock> client_;
  SymmetricDeltaServerMock service_;
};

using ::testing::_;
using ::testing::Return;

TEST_F(OsiGrpcSymmetricDeltaTest, SymmetricDelta) {
  EXPECT_CALL(service_, SymmetricDelta(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->SymmetricDelta());
}

}  // namespace
