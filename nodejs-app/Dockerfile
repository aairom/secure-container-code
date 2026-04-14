# Multi-stage build for Node.js application with obfuscation
# Stage 1: Build and obfuscate the application
FROM node:20-alpine AS builder

# Set working directory
WORKDIR /build

# Copy package files
COPY package.json ./

# Install ALL dependencies (including devDependencies for obfuscation)
RUN npm install

# Copy source code
COPY app.js ./

# Create dist directory and obfuscate the code
RUN mkdir -p dist && \
    npx javascript-obfuscator app.js \
    --output dist/app.js \
    --compact true \
    --control-flow-flattening true \
    --control-flow-flattening-threshold 0.75 \
    --dead-code-injection true \
    --dead-code-injection-threshold 0.4 \
    --string-array true \
    --string-array-encoding 'base64' \
    --string-array-threshold 0.75 \
    --transform-object-keys true \
    --unicode-escape-sequence true

# Create production package.json (without devDependencies)
RUN npm prune --production && \
    cp package.json dist/

# Stage 2: Create minimal runtime image
FROM node:20-alpine

# Install dumb-init for proper signal handling
RUN apk add --no-cache dumb-init

# Create non-root user
RUN addgroup -g 1001 -S nodejs && \
    adduser -S nodejs -u 1001

# Set working directory
WORKDIR /app

# Copy package.json from builder
COPY --from=builder /build/dist/package.json ./

# Copy production node_modules from builder
COPY --from=builder /build/node_modules ./node_modules

# Copy obfuscated application from builder
COPY --from=builder /build/dist/app.js ./

# Change ownership to non-root user
RUN chown -R nodejs:nodejs /app

# Switch to non-root user
USER nodejs

# Expose port
EXPOSE 8080

# Set environment variables
ENV NODE_ENV=production
ENV PORT=8080

# Use dumb-init to handle signals properly
ENTRYPOINT ["dumb-init", "--"]

# Run the obfuscated application
CMD ["node", "app.js"]

# Benefits of this approach:
# 1. Source code is heavily obfuscated using javascript-obfuscator
# 2. Original source code is NOT in the final image
# 3. Obfuscation tools are NOT in the final image
# 4. Only production dependencies in final image
# 5. Non-root user for security
# 6. Minimal Alpine-based image (~50MB)
# 7. Code is transformed with multiple obfuscation techniques:
#    - Control flow flattening
#    - Dead code injection
#    - String array encoding
#    - Object key transformation
#    - Unicode escape sequences
#
# Note: While obfuscation makes reverse engineering significantly harder,
# it's not foolproof. However, it raises the bar considerably.