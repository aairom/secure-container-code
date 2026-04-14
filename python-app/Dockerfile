# Multi-stage build for Python application with obfuscation
# Stage 1: Build and obfuscate the application
FROM python:3.11-slim AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy requirements and install dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy source code
COPY app.py .

# Obfuscate the Python code using PyArmor
# This protects the source code from being easily read
RUN pyarmor gen -O dist app.py

# Create a minimal requirements.txt for runtime (without pyarmor)
RUN echo "Flask==3.0.0" > dist/requirements.txt && \
    echo "Werkzeug==3.0.1" >> dist/requirements.txt

# Stage 2: Create minimal runtime image
FROM python:3.11-slim

# Install runtime dependencies only
WORKDIR /app

# Copy requirements from builder
COPY --from=builder /build/dist/requirements.txt .

# Install runtime dependencies
RUN pip install --no-cache-dir -r requirements.txt && \
    rm requirements.txt

# Copy obfuscated application from builder
COPY --from=builder /build/dist/ .

# Create non-root user
RUN useradd -r -u 1001 appuser && \
    chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Expose port
EXPOSE 8080

# Set environment variables
ENV PYTHONUNBUFFERED=1
ENV PORT=8080

# Run the obfuscated application
CMD ["python", "app.py"]

# Benefits of this approach:
# 1. Source code is obfuscated using PyArmor
# 2. Original source code is NOT in the final image
# 3. PyArmor obfuscation tool is NOT in the final image
# 4. Only runtime dependencies in final image
# 5. Non-root user for security
# 6. Bytecode is encrypted and harder to decompile
# 7. Smaller final image (~150MB vs 1GB+ with build tools)
#
# Note: While obfuscation makes reverse engineering harder,
# it's not foolproof. Determined attackers can still analyze
# the obfuscated code, but it significantly raises the bar.