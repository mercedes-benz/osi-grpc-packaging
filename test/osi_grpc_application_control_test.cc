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

#include "test/osi_grpc_application_control_mock.h"

namespace {

class OsiGrpcApplicationControlTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto builder = grpc::ServerBuilder{};
    builder.RegisterService(&service_);
    server_ = builder.BuildAndStart();

    auto channel_args = grpc::ChannelArguments{};
    auto channel = server_->InProcessChannel(channel_args);

    server_thread_ = std::make_unique<std::thread>([this]() { server_->Wait(); });
    client_ = std::make_unique<ApplicationControlClientMock>(channel);
  }

  void TearDown() override {
    server_->Shutdown();
    server_thread_->join();
  }

  ApplicationControlServerMock service_;
  std::unique_ptr<ApplicationControlClientMock> client_;
  std::unique_ptr<grpc::Server> server_;
  std::unique_ptr<std::thread> server_thread_;
};

using ::testing::_;
using ::testing::Return;

TEST_F(OsiGrpcApplicationControlTest, Initialize) {
  EXPECT_CALL(service_, Initialize(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->Initialize());
}

TEST_F(OsiGrpcApplicationControlTest, Start) {
  EXPECT_CALL(service_, Start(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->Start());
}

TEST_F(OsiGrpcApplicationControlTest, Pause) {
  EXPECT_CALL(service_, Pause(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->Pause());
}

TEST_F(OsiGrpcApplicationControlTest, Stop) {
  EXPECT_CALL(service_, Stop(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->Stop());
}

TEST_F(OsiGrpcApplicationControlTest, Log) {
  EXPECT_CALL(service_, Log(_, _, _)).Times(1).WillOnce(Return(grpc::Status::OK));
  ASSERT_TRUE(client_->Log());
}

}  // namespace
