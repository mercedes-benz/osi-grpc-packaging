FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.25.2"
ARG INSTALL_PROTOLINT_VERSION_FROM_SOURCE="0.42.2"

COPY ./reinstall-cmake.sh ./install-protolint.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
    chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN if [ "${INSTALL_PROTOLINT_VERSION_FROM_SOURCE}" != "none" ]; then \
    chmod +x /tmp/install-protolint.sh && /tmp/install-protolint.sh ${INSTALL_PROTOLINT_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/install-protolint.sh

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    clang-format-15 \
    clang-tidy-15 \
    doxygen \
    gcc-12 \
    graphviz \
    libgmock-dev \
    libgrpc++-dev \
    libgtest-dev \
    libprotobuf-dev \
    protobuf-compiler \
    protobuf-compiler-grpc \
    python3-pip

RUN pip install --no-cache-dir cmakelang
