FROM ubuntu:25.04

LABEL maintainer="jaehyung"
LABEL description="C++20 development environment with GCC 15, Clang 20, CMake, and Ninja"

ENV DEBIAN_FRONTEND=noninteractive \
    CC=gcc-15 \
    CXX=g++-15 \
    BUILD_TYPE=Release

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-15 g++-15 \
    clang-20 clang++-20 clang-format-20 clang-tidy-20 \
    cmake ninja-build \
    gdb valgrind \
    git curl wget \
    ca-certificates \
    pkg-config \
    python3 python3-pip \
 && rm -rf /var/lib/apt/lists/*

RUN ln -sf /usr/bin/clang-format-20 /usr/bin/clang-format \
 && ln -sf /usr/bin/clang-tidy-20 /usr/bin/clang-tidy

WORKDIR /workspace

RUN git config --global --add safe.directory /workspace

CMD ["/bin/bash"]
