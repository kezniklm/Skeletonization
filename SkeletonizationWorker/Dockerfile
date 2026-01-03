# Skeletonization Worker Dockerfile

FROM nvidia/cuda:12.9.0-devel-ubuntu22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    wget \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

# Copy project files
WORKDIR /build
COPY . .

# Configure and build
RUN cmake --preset default -DSKELETONIZATION_ENABLE_GPU=ON
RUN cmake --build --preset default --target SkeletonizationWorker

# Runtime stage
FROM nvidia/cuda:12.9.0-runtime-ubuntu22.04

# Install runtime dependencies (including OpenSSL for Redis SSL support)
RUN apt-get update && apt-get install -y \
    libopencv-core4.5d \
    libopencv-imgcodecs4.5d \
    libopencv-imgproc4.5d \
    libgoogle-glog0v5 \
    libhiredis0.14 \
    libssl3 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 worker && \
    mkdir -p /app/output && \
    chown -R worker:worker /app

USER worker
WORKDIR /app

# Copy built executable
COPY --from=builder --chown=worker:worker /build/build/SkeletonizationWorker/SkeletonizationWorker /app/

# Set default environment variables
# REDIS_URL format: redis[s]://[[username:]password@]host[:port][/database]
# Use 'rediss://' for SSL connections
ENV REDIS_URL=redis://redis:6379/0
ENV OUTPUT_DIRECTORY=/app/output
ENV PROCESSOR_TYPE=gpu

# Create output directory volume
VOLUME ["/app/output"]

CMD ["./SkeletonizationWorker"]
