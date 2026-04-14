/**
 * Secure Node.js Application with Proprietary Algorithm
 * This code will be obfuscated before containerization
 */

const express = require('express');
const crypto = require('crypto');

// Secret proprietary constants - will be obfuscated
const SECRET_KEY = 'nodejs-proprietary-algorithm-2024';
const ITERATIONS = 7000;

/**
 * Proprietary Algorithm Class
 * Contains our secret business logic that will be protected
 */
class ProprietaryAlgorithm {
    constructor() {
        this.secretSalt = SECRET_KEY;
        this.iterations = ITERATIONS;
        this.internalState = this._initializeState();
    }

    /**
     * Internal initialization - will be obfuscated
     */
    _initializeState() {
        return crypto.createHash('sha256')
            .update(this.secretSalt)
            .digest('hex');
    }

    /**
     * Proprietary transformation logic
     * This is our "secret sauce" that will be protected
     */
    _applyTransformation(data, iteration) {
        const combined = `${data}${this.internalState}${iteration}`;
        return crypto.createHash('sha256')
            .update(combined)
            .digest('hex');
    }

    /**
     * Main processing method with proprietary algorithm
     * Multiple iterations make reverse engineering harder
     */
    processData(inputData) {
        let result = inputData + this.secretSalt;
        
        // Apply our secret transformation multiple times
        for (let i = 0; i < this.iterations; i++) {
            result = this._applyTransformation(result, i);
        }
        
        return result;
    }
}

/**
 * License Validator Class
 * Validates application licensing - will be obfuscated
 */
class LicenseValidator {
    static validate() {
        const licenseKey = process.env.LICENSE_KEY || '';
        
        if (!licenseKey) {
            console.log('Warning: No license key provided - running in DEMO mode');
            return false;
        }
        
        // Simulated license validation
        const expectedHash = 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855';
        const actualHash = crypto.createHash('sha256')
            .update(licenseKey)
            .digest('hex');
        
        const isValid = actualHash === expectedHash || licenseKey === 'DEMO-LICENSE-KEY';
        
        if (isValid) {
            console.log('License validated successfully');
        } else {
            console.log('Invalid license - some features may be limited');
        }
        
        return isValid;
    }
}

// Initialize Express app
const app = express();
app.use(express.json());

// Initialize our proprietary algorithm
const algorithm = new ProprietaryAlgorithm();

// Validate license on startup
const licenseValid = LicenseValidator.validate();

/**
 * Health check endpoint
 */
app.get('/health', (req, res) => {
    res.json({
        status: 'healthy',
        timestamp: new Date().toISOString(),
        licensed: licenseValid
    });
});

/**
 * Process data using our proprietary algorithm
 */
app.post('/process', (req, res) => {
    const data = req.query.data;
    
    if (!data) {
        return res.status(400).json({
            error: 'Missing "data" parameter'
        });
    }
    
    // Apply our proprietary algorithm
    const result = algorithm.processData(data);
    
    res.json({
        input: data,
        processed: result,
        algorithm: 'nodejs-proprietary-v1'
    });
});

/**
 * Root endpoint
 */
app.get('/', (req, res) => {
    res.json({
        service: 'Secure Node.js Application',
        endpoints: {
            '/health': 'GET - Health check',
            '/process': 'POST - Process data (requires ?data=value)'
        }
    });
});

// Start server
const PORT = process.env.PORT || 8080;

app.listen(PORT, '0.0.0.0', () => {
    console.log(`Starting Node.js secure application on port ${PORT}`);
    console.log('Endpoints: /health (GET), /process (POST)');
});

// Handle graceful shutdown
process.on('SIGTERM', () => {
    console.log('SIGTERM signal received: closing HTTP server');
    process.exit(0);
});

process.on('SIGINT', () => {
    console.log('SIGINT signal received: closing HTTP server');
    process.exit(0);
});

// Made with Bob
