# Mermaid Diagrams - Visual Architecture

This document contains all Mermaid diagrams for visualizing the secure containerization architecture and workflows.

## Table of Contents

1. [Multi-Stage Build Process](#1-multi-stage-build-process)
2. [Source Code Protection Flow](#2-source-code-protection-flow)
3. [Security Layers](#3-security-layers)
4. [Attack Vectors and Mitigations](#4-attack-vectors-and-mitigations)
5. [Language Comparison](#5-language-comparison)
6. [Container Build Workflow](#6-container-build-workflow)
7. [Runtime Architecture](#7-runtime-architecture)
8. [Reverse Engineering Difficulty](#8-reverse-engineering-difficulty)
9. [Decision Tree](#9-decision-tree)
10. [Protection Techniques](#10-protection-techniques)

---

## 1. Multi-Stage Build Process

### General Multi-Stage Build Flow

```mermaid
graph TB
    A[Source Code] --> B[Stage 1: Builder]
    B --> C{Language Type?}
    
    C -->|Compiled| D[Compile to Binary]
    C -->|Interpreted| E[Obfuscate Code]
    
    D --> F[Optimize & Strip]
    E --> G[Encrypt/Transform]
    
    F --> H[Stage 2: Runtime]
    G --> H
    
    H --> I[Minimal Base Image]
    I --> J[Copy Binary/Obfuscated Code]
    J --> K[Non-root User]
    K --> L[Final Secure Image]
    
    style A fill:#e1f5ff
    style L fill:#c8e6c9
    style B fill:#fff9c4
    style H fill:#fff9c4
```

### Go Multi-Stage Build

```mermaid
graph LR
    A[main.go] --> B[golang:1.21-alpine]
    B --> C[go build -ldflags='-s -w']
    C --> D[Static Binary]
    D --> E[distroless/static]
    E --> F[Final Image ~4MB]
    
    style A fill:#00ADD8
    style F fill:#00ADD8
```

### C++ Multi-Stage Build

```mermaid
graph LR
    A[main.cpp] --> B[gcc:13-bookworm]
    B --> C[g++ -O3 -s]
    C --> D[Optimized Binary]
    D --> E[debian:bookworm-slim]
    E --> F[Final Image ~100MB]
    
    style A fill:#00599C
    style F fill:#00599C
```

### Python Multi-Stage Build

```mermaid
graph LR
    A[app.py] --> B[python:3.11-slim]
    B --> C[pyarmor gen]
    C --> D[Obfuscated Code]
    D --> E[python:3.11-slim]
    E --> F[Final Image ~150MB]
    
    style A fill:#3776AB
    style F fill:#3776AB
```

### Node.js Multi-Stage Build

```mermaid
graph LR
    A[app.js] --> B[node:20-alpine]
    B --> C[javascript-obfuscator]
    C --> D[Obfuscated Code]
    D --> E[node:20-alpine]
    E --> F[Final Image ~50MB]
    
    style A fill:#339933
    style F fill:#339933
```

---

## 2. Source Code Protection Flow

```mermaid
flowchart TD
    A[Original Source Code] --> B{Protection Strategy}
    
    B -->|Compiled Languages| C[Compilation]
    B -->|Interpreted Languages| D[Obfuscation]
    
    C --> E[Binary Generation]
    E --> F[Symbol Stripping]
    F --> G[Optimization]
    
    D --> H[Code Transformation]
    H --> I[Encryption/Encoding]
    I --> J[Runtime Protection]
    
    G --> K[Containerization]
    J --> K
    
    K --> L[Multi-Stage Build]
    L --> M[Remove Build Tools]
    M --> N[Minimal Runtime]
    N --> O[Non-root User]
    O --> P[Final Secure Container]
    
    P --> Q{Security Level}
    Q -->|Go/C++| R[Excellent Protection]
    Q -->|Python| S[Good Protection]
    Q -->|Node.js| T[Moderate Protection]
    
    style A fill:#ffcdd2
    style P fill:#c8e6c9
    style R fill:#4caf50
    style S fill:#8bc34a
    style T fill:#cddc39
```

---

## 3. Security Layers

```mermaid
graph TB
    subgraph "Layer 4: Legal Protection"
        L1[Copyright]
        L2[Patents]
        L3[EULA]
        L4[Trade Secrets]
    end
    
    subgraph "Layer 3: Operational Security"
        O1[Access Control]
        O2[Private Registry]
        O3[Monitoring]
        O4[Audit Logs]
    end
    
    subgraph "Layer 2: Container Security"
        C1[Multi-stage Build]
        C2[Minimal Image]
        C3[Non-root User]
        C4[Read-only FS]
    end
    
    subgraph "Layer 1: Code Protection"
        P1[Compilation/Obfuscation]
        P2[Symbol Stripping]
        P3[Encryption]
        P4[Anti-debugging]
    end
    
    L1 --> O1
    L2 --> O2
    L3 --> O3
    L4 --> O4
    
    O1 --> C1
    O2 --> C2
    O3 --> C3
    O4 --> C4
    
    C1 --> P1
    C2 --> P2
    C3 --> P3
    C4 --> P4
    
    style L1 fill:#e3f2fd
    style L2 fill:#e3f2fd
    style L3 fill:#e3f2fd
    style L4 fill:#e3f2fd
    style O1 fill:#fff3e0
    style O2 fill:#fff3e0
    style O3 fill:#fff3e0
    style O4 fill:#fff3e0
    style C1 fill:#f3e5f5
    style C2 fill:#f3e5f5
    style C3 fill:#f3e5f5
    style C4 fill:#f3e5f5
    style P1 fill:#e8f5e9
    style P2 fill:#e8f5e9
    style P3 fill:#e8f5e9
    style P4 fill:#e8f5e9
```

---

## 4. Attack Vectors and Mitigations

```mermaid
graph TD
    A[Attacker] --> B{Attack Vector}
    
    B -->|Container Extraction| C[docker save/export]
    B -->|Static Analysis| D[Binary Analysis Tools]
    B -->|Dynamic Analysis| E[Debuggers/Tracers]
    B -->|Memory Dumping| F[Process Memory Dump]
    
    C --> G{Mitigation}
    D --> G
    E --> G
    F --> G
    
    G -->|Go/C++| H[Compiled Binary]
    G -->|Python| I[PyArmor Encryption]
    G -->|Node.js| J[Heavy Obfuscation]
    
    H --> K[Symbol Stripping]
    I --> K
    J --> K
    
    K --> L[Minimal Base Image]
    L --> M[No Debug Tools]
    M --> N[Non-root User]
    N --> O[Runtime Checks]
    
    O --> P{Result}
    P -->|Go/C++| Q[Very Hard to Reverse]
    P -->|Python| R[Hard to Reverse]
    P -->|Node.js| S[Moderate to Reverse]
    
    style A fill:#ffcdd2
    style Q fill:#4caf50
    style R fill:#8bc34a
    style S fill:#cddc39
```

---

## 5. Language Comparison

### Protection Level Comparison

```mermaid
graph LR
    A[Protection Level] --> B[Go: ⭐⭐⭐⭐⭐]
    A --> C[C++: ⭐⭐⭐⭐⭐]
    A --> D[Python: ⭐⭐⭐⭐]
    A --> E[Node.js: ⭐⭐⭐]
    
    style B fill:#4caf50
    style C fill:#4caf50
    style D fill:#8bc34a
    style E fill:#cddc39
```

### Image Size vs Security

```mermaid
quadrantChart
    title Image Size vs Security Level
    x-axis Small Image --> Large Image
    y-axis Low Security --> High Security
    quadrant-1 Ideal Zone
    quadrant-2 Over-engineered
    quadrant-3 Needs Improvement
    quadrant-4 Good Balance
    Go: [0.1, 0.95]
    Node.js: [0.3, 0.6]
    C++: [0.5, 0.95]
    Python: [0.7, 0.75]
```

---

## 6. Container Build Workflow

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant Docker as Docker Build
    participant Builder as Builder Stage
    participant Runtime as Runtime Stage
    participant Registry as Container Registry
    
    Dev->>Docker: docker build
    Docker->>Builder: Create builder container
    Builder->>Builder: Install dependencies
    Builder->>Builder: Copy source code
    
    alt Compiled Language
        Builder->>Builder: Compile to binary
        Builder->>Builder: Strip symbols
    else Interpreted Language
        Builder->>Builder: Obfuscate code
        Builder->>Builder: Encrypt/transform
    end
    
    Docker->>Runtime: Create runtime container
    Runtime->>Runtime: Use minimal base
    Builder->>Runtime: Copy binary/obfuscated code
    Runtime->>Runtime: Set non-root user
    Runtime->>Runtime: Configure entrypoint
    
    Docker->>Registry: Push final image
    Registry-->>Dev: Image ready
    
    Note over Builder,Runtime: Source code never in final image
```

---

## 7. Runtime Architecture

```mermaid
graph TB
    subgraph "Container Runtime"
        A[Entrypoint] --> B{Application Type}
        
        B -->|Go| C[Static Binary]
        B -->|C++| D[Native Binary]
        B -->|Python| E[Obfuscated .py]
        B -->|Node.js| F[Obfuscated .js]
        
        C --> G[No Dependencies]
        D --> H[Runtime Libraries Only]
        E --> I[Python Runtime]
        F --> J[Node.js Runtime]
        
        G --> K[Process Execution]
        H --> K
        I --> K
        J --> K
        
        K --> L[License Validation]
        L --> M[API Server]
        M --> N[Request Processing]
        N --> O[Proprietary Algorithm]
        O --> P[Response]
    end
    
    subgraph "Security Features"
        Q[Non-root User]
        R[Read-only FS]
        S[No Shell]
        T[Minimal Attack Surface]
    end
    
    K -.-> Q
    K -.-> R
    K -.-> S
    K -.-> T
    
    style C fill:#00ADD8
    style D fill:#00599C
    style E fill:#3776AB
    style F fill:#339933
```

---

## 8. Reverse Engineering Difficulty

```mermaid
graph TD
    A[Reverse Engineering Attempt] --> B{Target Language}
    
    B -->|Go/C++| C[Binary Analysis Required]
    B -->|Python| D[Bytecode Analysis Required]
    B -->|Node.js| E[Code Deobfuscation Required]
    
    C --> F[Tools: IDA Pro, Ghidra]
    D --> G[Tools: PyArmor Bypass]
    E --> H[Tools: Beautifiers, AST]
    
    F --> I[Skill: Expert]
    G --> J[Skill: Advanced]
    H --> K[Skill: Intermediate]
    
    I --> L[Time: Weeks]
    J --> M[Time: Days]
    K --> N[Time: Hours-Days]
    
    L --> O[Success Rate: Low]
    M --> P[Success Rate: Medium]
    N --> Q[Success Rate: High]
    
    style C fill:#4caf50
    style D fill:#8bc34a
    style E fill:#cddc39
    style O fill:#4caf50
    style P fill:#ff9800
    style Q fill:#f44336
```

---

## 9. Decision Tree

```mermaid
graph TD
    A[Choose Language for Secure Container] --> B{Primary Requirement?}
    
    B -->|Maximum Security| C{Performance Critical?}
    B -->|Rapid Development| D{Ecosystem Needed?}
    B -->|Small Image Size| E[Go]
    
    C -->|Yes| F[C++]
    C -->|No| E
    
    D -->|Python| G[Python + PyArmor]
    D -->|JavaScript| H[Node.js + Obfuscation]
    
    E --> I[Excellent Protection<br/>~4MB Image]
    F --> J[Excellent Protection<br/>~100MB Image]
    G --> K[Good Protection<br/>~150MB Image]
    H --> L[Moderate Protection<br/>~50MB Image]
    
    I --> M{Acceptable?}
    J --> M
    K --> M
    L --> M
    
    M -->|Yes| N[Implement Solution]
    M -->|No| O[Consider SaaS Model]
    
    style E fill:#00ADD8
    style F fill:#00599C
    style G fill:#3776AB
    style H fill:#339933
    style N fill:#4caf50
    style O fill:#2196f3
```

---

## 10. Protection Techniques

### Compilation Process (Go/C++)

```mermaid
flowchart LR
    A[Source Code] --> B[Lexical Analysis]
    B --> C[Syntax Analysis]
    C --> D[Semantic Analysis]
    D --> E[Intermediate Code]
    E --> F[Optimization]
    F --> G[Code Generation]
    G --> H[Machine Code]
    H --> I[Symbol Stripping]
    I --> J[Final Binary]
    
    style A fill:#ffcdd2
    style J fill:#c8e6c9
```

### Obfuscation Process (Python/Node.js)

```mermaid
flowchart TD
    A[Source Code] --> B[Parse AST]
    B --> C{Obfuscation Techniques}
    
    C --> D[Variable Renaming]
    C --> E[String Encoding]
    C --> F[Control Flow Flattening]
    C --> G[Dead Code Injection]
    C --> H[Object Key Transform]
    
    D --> I[Combine Transformations]
    E --> I
    F --> I
    G --> I
    H --> I
    
    I --> J[Generate Obfuscated Code]
    J --> K[Add Runtime Protection]
    K --> L[Final Obfuscated Code]
    
    style A fill:#ffcdd2
    style L fill:#c8e6c9
```

### Complete Protection Stack

```mermaid
graph TB
    subgraph "Development Phase"
        A1[Write Code]
        A2[Add License Checks]
        A3[Implement Anti-debugging]
    end
    
    subgraph "Build Phase"
        B1[Multi-stage Build]
        B2[Compile/Obfuscate]
        B3[Strip Symbols]
        B4[Optimize]
    end
    
    subgraph "Container Phase"
        C1[Minimal Base Image]
        C2[Non-root User]
        C3[No Build Tools]
        C4[Read-only FS]
    end
    
    subgraph "Distribution Phase"
        D1[Private Registry]
        D2[Image Signing]
        D3[Access Control]
        D4[Audit Logging]
    end
    
    subgraph "Runtime Phase"
        E1[License Validation]
        E2[Integrity Checks]
        E3[Anti-tampering]
        E4[Monitoring]
    end
    
    A1 --> B1
    A2 --> B2
    A3 --> B3
    
    B1 --> C1
    B2 --> C2
    B3 --> C3
    B4 --> C4
    
    C1 --> D1
    C2 --> D2
    C3 --> D3
    C4 --> D4
    
    D1 --> E1
    D2 --> E2
    D3 --> E3
    D4 --> E4
    
    style A1 fill:#e3f2fd
    style B1 fill:#fff3e0
    style C1 fill:#f3e5f5
    style D1 fill:#fce4ec
    style E1 fill:#e8f5e9
```

---

## Usage in Documentation

These diagrams can be rendered in any Markdown viewer that supports Mermaid, including:
- GitHub
- GitLab
- VS Code (with Mermaid extension)
- Online Mermaid editors

To view these diagrams:
1. Copy the Mermaid code
2. Paste into a Mermaid-compatible viewer
3. Or view directly in GitHub/GitLab

## Diagram Legend

- 🟢 Green: Secure/Protected
- 🟡 Yellow: Moderate Protection
- 🔴 Red: Vulnerable/Unprotected
- 🔵 Blue: Process/Action
- ⚪ White: Neutral/Information

## Additional Resources

- [Mermaid Documentation](https://mermaid.js.org/)
- [Mermaid Live Editor](https://mermaid.live/)
- [GitHub Mermaid Support](https://github.blog/2022-02-14-include-diagrams-markdown-files-mermaid/)