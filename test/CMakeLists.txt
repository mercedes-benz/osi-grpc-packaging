#-------------------------------------------------------------------------------
# Copyright (c) 2023, MBition GmbH
#
# This program and the accompanying materials are made
# available under the terms of the Mozilla Public License Version 2.0
# which is available at https://www.mozilla.org/en-US/MPL/2.0/
#
# SPDX-License-Identifier: MPL-2.0
#-------------------------------------------------------------------------------

find_package(GTest REQUIRED)

add_executable(OsiGrpcApplicationControlTest)
target_sources(OsiGrpcApplicationControlTest PRIVATE osi_grpc_application_control_test.cc)
target_include_directories(OsiGrpcApplicationControlTest PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_features(OsiGrpcApplicationControlTest PRIVATE cxx_std_17)
target_link_libraries(OsiGrpcApplicationControlTest PRIVATE OsiGrpcPackaging::OsiGrpcPackaging GTest::gmock
                                                            GTest::gtest_main)

add_executable(OsiGrpcSymmetricDeltaTest)
target_sources(OsiGrpcSymmetricDeltaTest PRIVATE osi_grpc_symmetric_delta_test.cc)
target_include_directories(OsiGrpcSymmetricDeltaTest PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_features(OsiGrpcSymmetricDeltaTest PRIVATE cxx_std_17)
target_link_libraries(OsiGrpcSymmetricDeltaTest PRIVATE OsiGrpcPackaging::OsiGrpcPackaging GTest::gmock
                                                        GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(OsiGrpcApplicationControlTest)
gtest_discover_tests(OsiGrpcSymmetricDeltaTest)
