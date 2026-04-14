package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"
)

// Secret business logic - this will be compiled and not visible in the container
const (
	secretKey       = "my-super-secret-algorithm-key-2024"
	licenseCheckURL = "internal-license-server"
)

// ProprietaryAlgorithm represents our "secret sauce"
type ProprietaryAlgorithm struct {
	secretSalt string
	iterations int
}

// NewAlgorithm creates a new instance with proprietary settings
func NewAlgorithm() *ProprietaryAlgorithm {
	return &ProprietaryAlgorithm{
		secretSalt: secretKey,
		iterations: 10000,
	}
}

// ProcessData applies our proprietary algorithm
func (pa *ProprietaryAlgorithm) ProcessData(input string) string {
	// This is our "secret" business logic
	result := input + pa.secretSalt
	for i := 0; i < pa.iterations; i++ {
		hash := sha256.Sum256([]byte(result))
		result = hex.EncodeToString(hash[:])
	}
	return result
}

// ValidateLicense checks if the application is properly licensed
func ValidateLicense() bool {
	// Simulated license validation
	licenseKey := os.Getenv("LICENSE_KEY")
	if licenseKey == "" {
		log.Println("Warning: No license key provided")
		return false
	}

	// In production, this would call a license server
	expectedHash := "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
	hash := sha256.Sum256([]byte(licenseKey))
	actualHash := hex.EncodeToString(hash[:])

	return actualHash == expectedHash || licenseKey == "DEMO-LICENSE-KEY"
}

func healthHandler(w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(http.StatusOK)
	fmt.Fprintf(w, `{"status":"healthy","timestamp":"%s"}`, time.Now().Format(time.RFC3339))
}

func processHandler(algo *ProprietaryAlgorithm) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodPost {
			http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
			return
		}

		input := r.URL.Query().Get("data")
		if input == "" {
			http.Error(w, "Missing 'data' parameter", http.StatusBadRequest)
			return
		}

		// Apply our proprietary algorithm
		result := algo.ProcessData(input)

		w.Header().Set("Content-Type", "application/json")
		fmt.Fprintf(w, `{"input":"%s","processed":"%s","algorithm":"proprietary-v1"}`, input, result)
	}
}

func main() {
	// Validate license on startup
	if !ValidateLicense() {
		log.Println("Running in DEMO mode - some features may be limited")
	} else {
		log.Println("License validated successfully")
	}

	// Initialize our proprietary algorithm
	algo := NewAlgorithm()

	// Setup HTTP server
	http.HandleFunc("/health", healthHandler)
	http.HandleFunc("/process", processHandler(algo))

	port := os.Getenv("PORT")
	if port == "" {
		port = "8080"
	}

	log.Printf("Starting Go secure application on port %s", port)
	log.Printf("Endpoints: /health (GET), /process (POST)")

	if err := http.ListenAndServe(":"+port, nil); err != nil {
		log.Fatal(err)
	}
}

// Made with Bob
