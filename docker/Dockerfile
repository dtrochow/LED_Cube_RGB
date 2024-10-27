FROM ubuntu:24.10

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y build-essential \
    python3 \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    git

# Install uncrustofy necessary dependencies
RUN apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Clone uncrustify from the official repository
RUN git clone --branch uncrustify-0.78.1 --depth 1 https://github.com/uncrustify/uncrustify.git /uncrustify

# Build and install uncrustify
RUN cd /uncrustify && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Clean up
RUN rm -rf /uncrustify
