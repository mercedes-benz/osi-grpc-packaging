#-------------------------------------------------------------------------------
# Copyright (c) 2023, MBition GmbH
#
# This program and the accompanying materials are made
# available under the terms of the Mozilla Public License Version 2.0
# which is available at https://www.mozilla.org/en-US/MPL/2.0/
#
# SPDX-License-Identifier: MPL-2.0
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.24.0)

project(
  OsiGrpcPackaging
  VERSION 0.1.0
  LANGUAGES CXX)

# Build options
option(OsiGrpcPackaging_BUILD_DOCS "Build docs" ${PROJECT_IS_TOP_LEVEL})
option(OsiGrpcPackaging_BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(OsiGrpcPackaging_ENABLE_WARNINGS "Enable compiler warnings" ${PROJECT_IS_TOP_LEVEL})
option(OsiGrpcPackaging_ENABLE_WERROR "Fail and stop if a compiler warning is triggered" ${PROJECT_IS_TOP_LEVEL})
option(OsiGrpcPackaging_gRPC_UNSECURE "" OFF)
option(OsiGrpcPackaging_INSTALL "Install the project" ${PROJECT_IS_TOP_LEVEL})

# Find dependencies
find_package(Protobuf REQUIRED VERSION 3)
find_package(PkgConfig REQUIRED)
if(OsiGrpcPackaging_gRPC_UNSECURE)
  pkg_check_modules(grpc++ REQUIRED IMPORTED_TARGET grpc++_unsecure)
else()
  pkg_check_modules(grpc++ REQUIRED IMPORTED_TARGET grpc++)
endif()
find_program(gRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED)

add_subdirectory(open-simulation-interface)

# Generate Protobuf sources
set(PROTOS osi_grpc_application_control.proto osi_grpc_symmetric_delta.proto)

protobuf_generate_cpp(
  PROTO_SRCS PROTO_HEADERS
  PROTOS ${PROTOS} osi_application_control.proto
  IMPORT_DIRS ${PROJECT_SOURCE_DIR}/open-simulation-interface ${PROJECT_BINARY_DIR}/open-simulation-interface)

# Generate gRPC sources
protobuf_generate_cpp(
  gRPC_SRCS gRPC_HEADERS
  PROTOS ${PROTOS}
  IMPORT_DIRS ${PROJECT_SOURCE_DIR}/open-simulation-interface ${PROJECT_BINARY_DIR}/open-simulation-interface
  LANGUAGE grpc
  PLUGIN protoc-gen-grpc=${gRPC_CPP_PLUGIN}
  GENERATE_EXTENSIONS ".grpc.pb.cc" ".grpc.pb.h")

add_library(OsiGrpcPackaging)
add_library(OsiGrpcPackaging::OsiGrpcPackaging ALIAS OsiGrpcPackaging)

target_sources(
  OsiGrpcPackaging
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ${PROJECT_BINARY_DIR}
         FILES
         ${PROTO_HEADERS}
         ${gRPC_HEADERS}
  PRIVATE ${PROTO_SRCS} ${gRPC_SRCS})

# target_include_directories(OsiGrpcPackaging PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>)

target_link_libraries(OsiGrpcPackaging PUBLIC protobuf::libprotobuf PkgConfig::grpc++
                                              open_simulation_interface::open_simulation_interface_obj)

if(OsiGrpcPackaging_INSTALL)
  set(OsiGrpcPackaging_CONFIG_INSTALL_DIR
      share/OsiGrpcPackaging/cmake
      CACHE PATH "Install path for package configs")
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/OsiGrpcPackagingConfig.cmake.in ${PROJECT_BINARY_DIR}/OsiGrpcPackagingConfig.cmake
    INSTALL_DESTINATION ${OsiGrpcPackaging_CONFIG_INSTALL_DIR}
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/OsiGrpcPackagingConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  install(FILES ${PROJECT_BINARY_DIR}/OsiGrpcPackagingConfig.cmake
                ${PROJECT_BINARY_DIR}/OsiGrpcPackagingConfigVersion.cmake
          DESTINATION ${OsiGrpcPackaging_CONFIG_INSTALL_DIR})

  install(
    TARGETS OsiGrpcPackaging open_simulation_interface_obj
    EXPORT OsiGrpcPackagingTargets
    FILE_SET HEADERS)

  install(
    EXPORT OsiGrpcPackagingTargets
    DESTINATION ${OsiGrpcPackaging_CONFIG_INSTALL_DIR}
    NAMESPACE OsiGrpcPackaging::)

  export(
    EXPORT OsiGrpcPackagingTargets
    NAMESPACE OsiGrpcPackaging::
    FILE ${PROJECT_BINARY_DIR}/OsiGrpcPackagingTargets.cmake)
endif()

if(OsiGrpcPackaging_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
