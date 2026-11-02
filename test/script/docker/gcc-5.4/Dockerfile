FROM gcc:5.4

# The CMake shipped with jessie is 3.0.2, while the tests require CMake 3.15+.
ENV CMAKE_VERSION=3.15.7

# cmake.org sometimes fails or stalls in CI; fall back to the official
# GitHub release mirror, which carries the same artifact.
RUN set -eux; \
    name="cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz"; \
    primary="https://cmake.org/files/v3.15/${name}"; \
    fallback="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${name}"; \
    fetch() { \
        wget -nv --timeout=30 --tries=3 "$1" -O /tmp/cmake.tar.gz \
        && tar -tzf /tmp/cmake.tar.gz > /dev/null; \
    }; \
    fetch "$primary" || fetch "$fallback"; \
    tar -xzf /tmp/cmake.tar.gz -C /usr/local --strip-components=1; \
    rm -f /tmp/cmake.tar.gz

WORKDIR /src
