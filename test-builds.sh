#!/bin/bash

# Test script to build all Docker images
# This verifies that all Dockerfiles work correctly

set -e  # Exit on error

echo "=========================================="
echo "Testing Docker Builds for All Applications"
echo "=========================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to build and test
build_app() {
    local app_name=$1
    local app_dir=$2
    
    echo -e "${YELLOW}Building ${app_name}...${NC}"
    cd "$app_dir"
    
    if docker build -t "secure-${app_dir}:latest" . ; then
        echo -e "${GREEN}✓ ${app_name} built successfully${NC}"
        
        # Get image size
        size=$(docker images "secure-${app_dir}:latest" --format "{{.Size}}")
        echo -e "  Image size: ${size}"
        echo ""
        return 0
    else
        echo -e "${RED}✗ ${app_name} build failed${NC}"
        echo ""
        return 1
    fi
    
    cd ..
}

# Track results
total=0
success=0
failed=0

# Build Go application
echo "1. Go Application"
echo "=================="
if build_app "Go Application" "go-app"; then
    ((success++))
else
    ((failed++))
fi
((total++))

# Build C++ application
echo "2. C++ Application"
echo "=================="
if build_app "C++ Application" "cpp-app"; then
    ((success++))
else
    ((failed++))
fi
((total++))

# Build Python application
echo "3. Python Application"
echo "====================="
if build_app "Python Application" "python-app"; then
    ((success++))
else
    ((failed++))
fi
((total++))

# Build Node.js application
echo "4. Node.js Application"
echo "======================"
if build_app "Node.js Application" "nodejs-app"; then
    ((success++))
else
    ((failed++))
fi
((total++))

# Summary
echo "=========================================="
echo "Build Summary"
echo "=========================================="
echo -e "Total:   ${total}"
echo -e "${GREEN}Success: ${success}${NC}"
if [ $failed -gt 0 ]; then
    echo -e "${RED}Failed:  ${failed}${NC}"
else
    echo -e "Failed:  ${failed}"
fi
echo ""

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}All builds completed successfully!${NC}"
    exit 0
else
    echo -e "${RED}Some builds failed. Please check the errors above.${NC}"
    exit 1
fi

# Made with Bob
