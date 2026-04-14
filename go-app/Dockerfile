# Multi-stage build for Go application
# Stage 1: Build the application
FROM golang:1.21-alpine AS builder

# Set working directory
WORKDIR /build

# Copy go mod files
COPY go.mod ./

# Download dependencies
RUN go mod download

# Copy source code
COPY main.go ./

# Build the application with optimizations
# -ldflags="-s -w" strips debug information and symbol table
# CGO_ENABLED=0 creates a static binary
RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build \
    -ldflags="-s -w -X main.buildTime=$(date -u +%Y-%m-%dT%H:%M:%SZ)" \
    -trimpath \
    -o secure-app \
    main.go

# Stage 2: Create minimal runtime image
# Using distroless for maximum security - no shell, no package manager
FROM gcr.io/distroless/static-debian11:nonroot

# Copy only the binary from builder
COPY --from=builder /build/secure-app /app/secure-app

# Use non-root user (already set in distroless/static:nonroot)
# UID 65532 is the nonroot user

# Expose port
EXPOSE 8080

# Set working directory
WORKDIR /app

# Run the application
ENTRYPOINT ["/app/secure-app"]

# Benefits of this approach:
# 1. Source code is NOT in the final image
# 2. No build tools in the final image
# 3. No shell access (distroless)
# 4. Minimal attack surface
# 5. Binary is stripped of debug symbols
# 6. Non-root user for security
# 7. Image size is minimal (~2MB for Go binary + ~2MB for distroless base)