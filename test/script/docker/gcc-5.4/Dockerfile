FROM gcc:5.4

# The CMake shipped with jessie is 3.0.2, while the tests require CMake 3.15+.
ENV CMAKE_VERSION=3.15.7

RUN wget -q "https://cmake.org/files/v3.15/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz" \
    -O /tmp/cmake.tar.gz \
    && tar -xzf /tmp/cmake.tar.gz -C /usr/local --strip-components=1 \
    && rm -f /tmp/cmake.tar.gz

WORKDIR /src
