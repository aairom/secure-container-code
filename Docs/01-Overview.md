# Secure Container Code - Overview

## Introduction

This project demonstrates how to protect source code in containerized applications across four popular programming languages: **Go**, **C++**, **Python**, and **Node.js**. Each implementation showcases best practices for minimizing source code exposure while maintaining functionality.

## Project Structure

```
secure-container-code/
├── go-app/              # Go application with multi-stage build
│   ├── main.go
│   ├── go.mod
│   ├── Dockerfile
│   └── .dockerignore
├── cpp-app/             # C++ application with compilation
│   ├── main.cpp
│   ├── Dockerfile
│   └── .dockerignore
├── python-app/          # Python application with obfuscation
│   ├── app.py
│   ├── requirements.txt
│   ├── Dockerfile
│   └── .dockerignore
├── nodejs-app/          # Node.js application with obfuscation
│   ├── app.js
│   ├── package.json
│   ├── Dockerfile
│   └── .dockerignore
├── k8s/                 # Kubernetes manifests
│   ├── go-app-deployment.yaml
│   ├── cpp-app-deployment.yaml
│   ├── python-app-deployment.yaml
│   ├── nodejs-app-deployment.yaml
│   ├── ingress.yaml
│   ├── network-policy.yaml
│   └── README.md
└── Docs/                # Documentation
    ├── 01-Overview.md
    ├── 02-Go-Application.md
    ├── 03-Cpp-Application.md
    ├── 04-Python-Application.md
    ├── 05-NodeJS-Application.md
    ├── 06-Security-Analysis.md
    └── 07-Mermaid-Diagrams.md
```

## Key Concepts

### 1. Multi-Stage Docker Builds

Multi-stage builds separate the build environment from the runtime environment:
- **Build Stage**: Contains compilers, build tools, and source code
- **Runtime Stage**: Contains only the compiled binary/obfuscated code and runtime dependencies

**Benefits:**
- Source code never reaches the final image
- Smaller image sizes
- Reduced attack surface
- No build tools in production

### 2. Code Obfuscation

For interpreted languages (Python, Node.js), obfuscation transforms code to make it harder to understand:
- **Python**: Uses PyArmor for bytecode encryption
- **Node.js**: Uses javascript-obfuscator for code transformation

**Techniques:**
- Control flow flattening
- Dead code injection
- String array encoding
- Variable name mangling
- Unicode escape sequences

### 3. Compilation

For compiled languages (Go, C++), source code is transformed into machine code:
- **Go**: Static binary compilation with symbol stripping
- **C++**: Native compilation with optimization and stripping

**Benefits:**
- Source code is not present in any form
- Reverse engineering requires advanced tools
- Better performance
- Smaller binaries

## Protection Levels by Language

| Language | Protection Method | Difficulty to Reverse | Image Size |
|----------|------------------|----------------------|------------|
| **Go** | Compiled binary + distroless | Very High | ~4MB |
| **C++** | Compiled binary + minimal base | Very High | ~100MB |
| **Python** | PyArmor obfuscation | Medium-High | ~150MB |
| **Node.js** | JavaScript obfuscation | Medium | ~50MB |

## Common Features

All four applications implement:

1. **HTTP REST API** with two endpoints:
   - `GET /health` - Health check
   - `POST /process?data=value` - Process data using proprietary algorithm

2. **Proprietary Algorithm**: Each app contains a "secret" business logic that:
   - Uses secret keys and constants
   - Applies multiple iterations of transformations
   - Demonstrates intellectual property protection

3. **License Validation**: Simulated license checking on startup

4. **Security Best Practices**:
   - Non-root user execution
   - Minimal base images
   - No shell access (where possible)
   - Read-only filesystem compatible
   - Environment variable configuration

## Important Limitations

⚠️ **No protection method is foolproof:**

1. **Binaries can be reverse-engineered** using tools like:
   - IDA Pro, Ghidra (for compiled code)
   - Decompilers and debuggers

2. **Obfuscated code can be analyzed** with:
   - Dynamic analysis
   - Memory dumps
   - Debugging tools

3. **Container images can be extracted**:
   - `docker save` / `docker export`
   - Layer inspection
   - Filesystem mounting

## Best Practices for Maximum Protection

1. **Technical Measures** (This Project):
   - Multi-stage builds
   - Code obfuscation/compilation
   - Minimal images
   - No debug symbols

2. **Legal Protection**:
   - Copyright notices
   - End-User License Agreements (EULA)
   - Terms of Service
   - Patent protection

3. **Business Model**:
   - SaaS instead of distribution
   - API-only access
   - License servers
   - Runtime validation

4. **Operational Security**:
   - Private container registries
   - Access controls
   - Audit logging
   - Regular security updates

## Use Cases

This approach is suitable for:

- **Commercial Software**: Protecting proprietary algorithms
- **Enterprise Applications**: Internal tools with sensitive logic
- **SaaS Products**: Backend services with competitive advantages
- **Licensed Software**: Applications requiring license enforcement

## Kubernetes Deployment

Production-ready Kubernetes manifests are available in the `k8s/` directory:

- **Deployments** - All 4 applications with security hardening
- **Services** - ClusterIP services for internal communication
- **HPA** - Horizontal Pod Autoscaling for all applications
- **Ingress** - Path and subdomain-based routing
- **Network Policies** - Traffic restriction and isolation
- **Security** - Non-root users, read-only FS, dropped capabilities

See [k8s/README.md](../k8s/README.md) for deployment instructions.

## Next Steps

Explore the detailed documentation for each language:
- [Go Application](./02-Go-Application.md)
- [C++ Application](./03-Cpp-Application.md)
- [Python Application](./04-Python-Application.md)
- [Node.js Application](./05-NodeJS-Application.md)
- [Security Analysis](./06-Security-Analysis.md)
- [Mermaid Diagrams](./07-Mermaid-Diagrams.md)
- [Kubernetes Deployment](../k8s/README.md)