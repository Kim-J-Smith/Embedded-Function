FROM gcc:5.4

# The CMake shipped with jessie is 3.0.2, while the tests require CMake 3.15+.
ENV CMAKE_VERSION=3.15.7

# Download the archive with curl, and check it against the published SHA-256
# (update the checksum together with CMAKE_VERSION).
#
# This is a 2017 jessie image: its wget is 1.16 and its trust store is old.
# wget is avoided because it decodes the percent-escaped URL that GitHub
# redirects to (turning "%2B" into "+"), so the storage backend sees a broken
# signature and answers 403 until wget gives up with "20 redirections
# exceeded". curl passes the URL through unchanged.
#
# TLS verification is disabled as well, because the old trust store cannot be
# relied on for today's certificates (cmake.org even served an expired one on
# 2026-11-15). Integrity is guaranteed by the pinned SHA-256 instead, so a
# tampered or truncated download always fails the build.
#
# GitHub releases are tried first because cmake.org is slow or unreachable
# from CI more often; both URLs serve the same artifact.
RUN set -eux; \
    name="cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz"; \
    sha256="8456a10ac2fd0dc97d13aa4753431a232cf32aecb0976541f7bf867a3f90ec57"; \
    github="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${name}"; \
    cmake_org="https://cmake.org/files/v3.15/${name}"; \
    fetch() { \
        curl -fsSLk --retry 3 --connect-timeout 30 --max-time 300 \
            -o "/tmp/${name}" "$1"; \
    }; \
    fetch "$github" || fetch "$cmake_org"; \
    echo "${sha256}  /tmp/${name}" | sha256sum -c -; \
    tar -xzf "/tmp/${name}" -C /usr/local --strip-components=1; \
    rm -f "/tmp/${name}"

WORKDIR /src
