FROM ubuntu:22.04

# Update the package list
RUN apt update

# Install build system dependencies
RUN apt install -y cmake build-essential upx python3 python3-pip git curl wget

# Install common libraries
RUN apt install -y libssl-dev libboost-all-dev rapidjson-dev libyaml-cpp-dev

# Install project specific libraries
RUN apt install -y libsdl2-dev
