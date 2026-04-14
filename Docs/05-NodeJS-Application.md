# Node.js Application - Secure Containerization with Obfuscation

## Overview

The Node.js application demonstrates **JavaScript obfuscation** using javascript-obfuscator to protect source code in containerized environments. The obfuscation applies multiple transformation techniques to make reverse engineering significantly more difficult.

## Application Features

### Proprietary Algorithm
- Secret key-based hashing with 7,000 iterations
- Node.js crypto module for SHA-256 transformations
- Business logic heavily obfuscated using javascript-obfuscator

### API Endpoints
- `GET /health` - Returns health status, timestamp, and license status
- `POST /process?data=value` - Processes data using proprietary algorithm
- `GET /` - Service information and endpoint list

### License Validation
- Checks `LICENSE_KEY` environment variable
- Validates against expected hash
- Runs in DEMO mode if no license provided

## File Structure

```
nodejs-app/
├── app.js              # Application source code
├── package.json        # Node.js dependencies and scripts
├── Dockerfile          # Multi-stage build with obfuscation
└── .dockerignore       # Files to exclude from build
```

## Source Code Highlights

### Proprietary Algorithm Implementation

```javascript
class ProprietaryAlgorithm {
    constructor() {
        this.secretSalt = SECRET_KEY;
        this.iterations = ITERATIONS;
        this.internalState = this._initializeState();
    }

    processData(inputData) {
        let result = inputData + this.secretSalt;
        for (let i = 0; i < this.iterations; i++) {
            result = this._applyTransformation(result, i);
        }
        return result;
    }
}
```

This code will be **heavily obfuscated** before containerization.

## JavaScript Obfuscation

### What is javascript-obfuscator?

A powerful tool that transforms JavaScript code through multiple techniques:

1. **Variable Renaming** - Meaningful names → random strings
2. **String Array Encoding** - Strings → encoded array lookups
3. **Control Flow Flattening** - Linear code → complex flow
4. **Dead Code Injection** - Add fake code paths
5. **Object Key Transformation** - Rename object properties
6. **Unicode Escape Sequences** - Convert strings to unicode

### Obfuscation Configuration

```javascript
// Configuration used in Dockerfile
{
  compact: true,                          // Remove whitespace
  controlFlowFlattening: true,            // Flatten control flow
  controlFlowFlatteningThreshold: 0.75,   // 75% of code affected
  deadCodeInjection: true,                // Add fake code
  deadCodeInjectionThreshold: 0.4,        // 40% dead code
  stringArray: true,                      // Encode strings
  stringArrayEncoding: 'base64',          // Base64 encoding
  stringArrayThreshold: 0.75,             // 75% strings encoded
  transformObjectKeys: true,              // Rename object keys
  unicodeEscapeSequence: true             // Unicode encoding
}
```

## Dockerfile Analysis

### Stage 1: Builder with Obfuscation

```dockerfile
FROM node:20-alpine AS builder

WORKDIR /build
COPY package.json ./
RUN npm install

COPY app.js ./

# Obfuscate the code with aggressive settings
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

# Prepare production dependencies
RUN npm prune --production && \
    cp package.json dist/
```

**Key steps:**
1. Install all dependencies (including obfuscator)
2. Apply aggressive obfuscation
3. Prune to production dependencies only
4. Obfuscator is NOT included in production

### Stage 2: Runtime

```dockerfile
FROM node:20-alpine

RUN apk add --no-cache dumb-init

RUN addgroup -g 1001 -S nodejs && \
    adduser -S nodejs -u 1001

WORKDIR /app

COPY --from=builder /build/dist/package.json ./
COPY --from=builder /build/node_modules ./node_modules
COPY --from=builder /build/dist/app.js ./

RUN chown -R nodejs:nodejs /app

USER nodejs

ENV NODE_ENV=production
ENV PORT=8080

ENTRYPOINT ["dumb-init", "--"]
CMD ["node", "app.js"]
```

**Runtime features:**
- Only obfuscated code (no original source)
- Obfuscation tool NOT included
- Non-root user execution
- Minimal Alpine-based image
- dumb-init for proper signal handling

## Building and Running

### Build the Docker Image

```bash
cd nodejs-app
docker build -t secure-nodejs-app:latest .
```

### Run the Container

```bash
# Basic run
docker run -p 8080:8080 secure-nodejs-app:latest

# With license key
docker run -p 8080:8080 \
  -e LICENSE_KEY=DEMO-LICENSE-KEY \
  secure-nodejs-app:latest

# With custom port
docker run -p 9000:9000 \
  -e PORT=9000 \
  secure-nodejs-app:latest

# Detached mode
docker run -d -p 8080:8080 --name nodejs-app secure-nodejs-app:latest
```

### Test the Application

```bash
# Health check
curl http://localhost:8080/health

# Process data
curl -X POST "http://localhost:8080/process?data=test123"

# Service info
curl http://localhost:8080/

# Expected response:
# {"input":"test123","processed":"<hash>","algorithm":"nodejs-proprietary-v1"}
```

## Security Analysis

### What's Protected

✅ **Source code**: Original `.js` files not in final image  
✅ **Variable names**: Renamed to meaningless strings  
✅ **String constants**: Encoded in base64 arrays  
✅ **Control flow**: Flattened and obfuscated  
✅ **Object keys**: Transformed and hidden  
✅ **Build tools**: Obfuscator not in final image  

### What's Still Vulnerable

⚠️ **JavaScript is interpreted**:
- Code must be readable by Node.js runtime
- Can be beautified and analyzed
- Dynamic analysis possible
- Debugger can inspect execution

⚠️ **Obfuscation can be reversed**:
- With significant effort and time
- Automated deobfuscation tools exist
- Manual analysis is possible
- Memory dumps reveal runtime state

⚠️ **Container extraction**:
```bash
docker save secure-nodejs-app:latest -o image.tar
```

## Image Size Comparison

| Stage | Size | Contents |
|-------|------|----------|
| Builder | ~200MB | Node.js, npm, obfuscator, source |
| Final | ~50MB | Node.js runtime + obfuscated code |
| **Reduction** | **75%** | Source and build tools eliminated |

## Obfuscation Effectiveness

### Before Obfuscation (Original)

```javascript
class ProprietaryAlgorithm {
    constructor() {
        this.secretSalt = SECRET_KEY;
        this.iterations = ITERATIONS;
    }
    
    processData(inputData) {
        let result = inputData + this.secretSalt;
        for (let i = 0; i < this.iterations; i++) {
            result = this._applyTransformation(result, i);
        }
        return result;
    }
}
```

### After Obfuscation (Simplified Example)

```javascript
var _0x4a2b=['constructor','secretSalt','iterations','processData','_applyTransformation'];
(function(_0x2d8f05,_0x4b81bb){var _0x4d74cb=function(_0x32719f){
while(--_0x32719f){_0x2d8f05['push'](_0x2d8f05['shift']());}};
_0x4d74cb(++_0x4b81bb);}(_0x4a2b,0x1f4));
var _0x4d74=function(_0x2d8f05,_0x4b81bb){_0x2d8f05=_0x2d8f05-0x0;
var _0x4d74cb=_0x4a2b[_0x2d8f05];return _0x4d74cb;};
class _0x3f2a8c{[_0x4d74('0x0')](){this[_0x4d74('0x1')]=_0x5e3c2a;
this[_0x4d74('0x2')]=_0x1a4b3d;}[_0x4d74('0x3')](_0x2e4f1c){
let _0x3d2a1b=_0x2e4f1c+this[_0x4d74('0x1')];
for(let _0x4c3b2d=0x0;_0x4c3b2d<this[_0x4d74('0x2')];_0x4c3b2d++){
_0x3d2a1b=this[_0x4d74('0x4')](_0x3d2a1b,_0x4c3b2d);}return _0x3d2a1b;}}
```

**Note**: Actual obfuscated code is even more complex with:
- Control flow flattening
- Dead code injection
- Multiple encoding layers
- Unicode escape sequences

## Verification Steps

### 1. Verify No Original Source

```bash
# Extract image
docker save secure-nodejs-app:latest -o nodejs-app.tar
mkdir nodejs-app-extracted
tar -xf nodejs-app.tar -C nodejs-app-extracted

# Search for original patterns
grep -r "class ProprietaryAlgorithm" nodejs-app-extracted
# Should not find readable class definition

# Check obfuscated code
find nodejs-app-extracted -name "app.js" -exec head -20 {} \;
# Should show obfuscated code
```

### 2. Verify Obfuscator Not in Runtime

```bash
# Check if obfuscator is present (should not be)
docker run secure-nodejs-app:latest npm list | grep obfuscator
# Should return nothing

# Check package.json
docker run secure-nodejs-app:latest cat package.json
# Should not list javascript-obfuscator
```

### 3. Compare File Sizes

```bash
# Original source
wc -l app.js
ls -lh app.js

# Extract obfuscated version
docker cp $(docker create secure-nodejs-app:latest):/app/app.js ./app_obfuscated.js

# Compare
wc -l app_obfuscated.js
ls -lh app_obfuscated.js
# Obfuscated version is typically larger
```

### 4. Attempt Beautification

```bash
# Try to beautify (will still be obfuscated)
npm install -g js-beautify
js-beautify app_obfuscated.js > app_beautified.js

# Compare - still hard to read
cat app_beautified.js
```

## Performance Characteristics

- **Startup time**: ~300ms
- **Memory usage**: ~30MB
- **Image size**: ~50MB
- **Build time**: ~60 seconds
- **Runtime overhead**: ~10-15% (due to obfuscation)

## Obfuscation Techniques Comparison

| Technique | Protection Level | Performance Impact | Reversibility |
|-----------|-----------------|-------------------|---------------|
| Variable Renaming | Low | None | Easy |
| String Encoding | Medium | Low | Medium |
| Control Flow Flattening | High | Medium | Hard |
| Dead Code Injection | Medium | Low | Medium |
| All Combined | **High** | **Medium** | **Very Hard** |

## Advantages of Node.js with Obfuscation

1. **Fast development** - JavaScript productivity
2. **Rich ecosystem** - npm packages
3. **Small images** - Alpine-based (~50MB)
4. **Good performance** - V8 engine optimization
5. **Easy deployment** - Simple containerization
6. **Reasonable protection** - Better than plain source
7. **Cross-platform** - Runs anywhere Node.js runs

## Limitations

1. **Not as secure as compiled languages** - JavaScript is interpreted
2. **Runtime overhead** - Obfuscation adds performance cost
3. **Deobfuscation possible** - With effort and tools
4. **Memory inspection** - Runtime state can be dumped
5. **Debugging possible** - Node.js debugger can attach

## Comparison with Other Languages

| Aspect | Node.js + Obfuscation | Python + PyArmor | Go/C++ |
|--------|----------------------|------------------|---------|
| Protection Level | Medium | Medium-High | Very High |
| Image Size | ~50MB | ~150MB | ~4-100MB |
| Build Time | ~60s | ~90s | ~30-60s |
| Performance | Good | Good | Excellent |
| Development Speed | Fast | Fast | Moderate |
| Reverse Engineering Difficulty | Medium | Medium-High | Very High |

## Alternative Obfuscation Tools

### 1. JScrambler
Commercial tool with advanced protection:
```bash
jscrambler -c config.json app.js
```
**Pros**: Very strong protection, anti-debugging  
**Cons**: Commercial license required, expensive

### 2. Webpack with Terser
Built-in minification and obfuscation:
```javascript
// webpack.config.js
optimization: {
  minimize: true,
  minimizer: [new TerserPlugin({
    terserOptions: {
      mangle: true,
      compress: true
    }
  })]
}
```
**Pros**: Free, integrated with build  
**Cons**: Limited protection compared to dedicated tools

### 3. pkg
Package Node.js app as executable:
```bash
pkg app.js --target node18-linux-x64
```
**Pros**: Single executable, harder to analyze  
**Cons**: Large files, limited obfuscation

## Best Practices Applied

1. ✅ **Multi-stage build** - Separates obfuscation and runtime
2. ✅ **Aggressive obfuscation** - Multiple techniques combined
3. ✅ **Minimal runtime** - Only production dependencies
4. ✅ **Non-root user** - Security by default
5. ✅ **No obfuscation tools** - Not in final image
6. ✅ **Alpine base** - Smallest possible image
7. ✅ **Signal handling** - dumb-init for proper shutdown

## Advanced Protection Techniques

### 1. Domain Locking
Restrict code to specific domains:
```javascript
const allowedDomains = ['example.com', 'api.example.com'];
if (!allowedDomains.includes(process.env.DOMAIN)) {
    process.exit(1);
}
```

### 2. Time Bombing
Expire code after certain date:
```javascript
const expiryDate = new Date('2024-12-31');
if (new Date() > expiryDate) {
    console.log('License expired');
    process.exit(1);
}
```

### 3. Anti-Debugging
Detect debugging attempts:
```javascript
const isDebug = () => {
    const start = Date.now();
    debugger;
    return Date.now() - start > 100;
};

if (isDebug()) {
    console.log('Debugger detected');
    process.exit(1);
}
```

### 4. Code Integrity Checks
Verify code hasn't been modified:
```javascript
const crypto = require('crypto');
const fs = require('fs');

const expectedHash = 'abc123...';
const actualHash = crypto.createHash('sha256')
    .update(fs.readFileSync(__filename))
    .digest('hex');

if (actualHash !== expectedHash) {
    process.exit(1);
}
```

## Recommendations

For maximum protection with Node.js:

1. **Combine techniques**:
   - Heavy obfuscation
   - License validation
   - Anti-debugging
   - Code integrity checks
   - Domain locking

2. **Legal protection**:
   - Copyright notices
   - EULA agreements
   - Terms of service
   - Patent protection

3. **Operational security**:
   - Private registries
   - Access controls
   - Audit logging
   - Regular updates

4. **Consider alternatives**:
   - SaaS model (don't distribute)
   - API-only access
   - Commercial obfuscators (JScrambler)
   - Hybrid approach (critical parts in native modules)

## Conclusion

The Node.js application provides **good source code protection** through:
- Aggressive JavaScript obfuscation
- Multiple transformation techniques
- No original source in final image
- No obfuscation tools in runtime
- Minimal Alpine-based image

Node.js with obfuscation is a **good choice** when:
- JavaScript ecosystem is required
- Development speed is important
- Moderate protection is acceptable
- Small image size is desired
- Team has JavaScript expertise

For **maximum protection**, consider:
- Compiled languages (Go, C++)
- Commercial obfuscators (JScrambler)
- SaaS deployment model
- Hybrid approach with native modules