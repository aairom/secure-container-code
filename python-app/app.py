"""
Secure Python Application with Proprietary Algorithm
This code will be obfuscated before containerization
"""

import hashlib
import os
import time
from flask import Flask, request, jsonify

# Secret proprietary constants - will be obfuscated
SECRET_KEY = "python-proprietary-algorithm-2024"
ITERATIONS = 8000


class ProprietaryAlgorithm:
    """
    Our secret business logic class
    This will be obfuscated to protect intellectual property
    """
    
    def __init__(self):
        self.secret_salt = SECRET_KEY
        self.iterations = ITERATIONS
        self._internal_state = self._initialize_state()
    
    def _initialize_state(self):
        """Internal initialization - will be obfuscated"""
        return hashlib.sha256(self.secret_salt.encode()).hexdigest()
    
    def _apply_transformation(self, data: str, iteration: int) -> str:
        """
        Proprietary transformation logic
        This is our "secret sauce" that will be protected
        """
        combined = f"{data}{self._internal_state}{iteration}"
        return hashlib.sha256(combined.encode()).hexdigest()
    
    def process_data(self, input_data: str) -> str:
        """
        Main processing method with proprietary algorithm
        Multiple iterations make reverse engineering harder
        """
        result = input_data + self.secret_salt
        
        # Apply our secret transformation multiple times
        for i in range(self.iterations):
            result = self._apply_transformation(result, i)
        
        return result


class LicenseValidator:
    """License validation logic - will be obfuscated"""
    
    @staticmethod
    def validate():
        """Check if application is properly licensed"""
        license_key = os.getenv('LICENSE_KEY', '')
        
        if not license_key:
            print("Warning: No license key provided - running in DEMO mode")
            return False
        
        # Simulated license validation
        expected_hash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
        actual_hash = hashlib.sha256(license_key.encode()).hexdigest()
        
        is_valid = actual_hash == expected_hash or license_key == "DEMO-LICENSE-KEY"
        
        if is_valid:
            print("License validated successfully")
        else:
            print("Invalid license - some features may be limited")
        
        return is_valid


# Initialize Flask app
app = Flask(__name__)

# Initialize our proprietary algorithm
algorithm = ProprietaryAlgorithm()

# Validate license on startup
license_valid = LicenseValidator.validate()


@app.route('/health', methods=['GET'])
def health_check():
    """Health check endpoint"""
    return jsonify({
        'status': 'healthy',
        'timestamp': time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()),
        'licensed': license_valid
    })


@app.route('/process', methods=['POST'])
def process_data():
    """Process data using our proprietary algorithm"""
    data = request.args.get('data')
    
    if not data:
        return jsonify({'error': 'Missing "data" parameter'}), 400
    
    # Apply our proprietary algorithm
    result = algorithm.process_data(data)
    
    return jsonify({
        'input': data,
        'processed': result,
        'algorithm': 'python-proprietary-v1'
    })


@app.route('/')
def index():
    """Root endpoint"""
    return jsonify({
        'service': 'Secure Python Application',
        'endpoints': {
            '/health': 'GET - Health check',
            '/process': 'POST - Process data (requires ?data=value)'
        }
    })


if __name__ == '__main__':
    port = int(os.getenv('PORT', 8080))
    print(f"Starting Python secure application on port {port}")
    print(f"Endpoints: /health (GET), /process (POST)")
    
    # Run with production settings
    app.run(host='0.0.0.0', port=port, debug=False)

# Made with Bob
