#!/usr/bin/env bash
#-------------------------------------------------------------------------------
# Copyright (c) 2023, MBition GmbH
#
# This program and the accompanying materials are made
# available under the terms of the Mozilla Public License Version 2.0
# which is available at https://www.mozilla.org/en-US/MPL/2.0/
#
# SPDX-License-Identifier: MPL-2.0
#-------------------------------------------------------------------------------

set -e

PROTOLINT_VERSION=${1:-"none"}

if [ "${PROTOLINT_VERSION}" = "none" ]; then
    echo "No protolint version specified, skipping protolint installation"
    exit 0
fi

echo "Installing protolint..."
mkdir -p /opt/protolint/bin

architecture=$(dpkg --print-architecture)
case "${architecture}" in
    arm64)
        ARCH=arm64 ;;
    amd64)
        ARCH=x86_64 ;;
    *)
        echo "Unsupported architecture ${architecture}."
        exit 1
        ;;
esac

PROTOLINT_BINARY_NAME="protolint_${PROTOLINT_VERSION}_Linux_${ARCH}.tar.gz"

curl -sSL "https://github.com/yoheimuta/protolint/releases/download/v${PROTOLINT_VERSION}/${PROTOLINT_BINARY_NAME}" | \
    tar xvz -C /opt/protolint/bin

ln -s /opt/protolint/bin/protolint /usr/local/bin/protolint
