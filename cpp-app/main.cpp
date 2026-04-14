#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <openssl/sha.h>
#include <microhttpd.h>

// Secret proprietary algorithm constants
const std::string SECRET_KEY = "cpp-proprietary-algorithm-2024";
const int ITERATIONS = 5000;

// Proprietary algorithm class - will be compiled to binary
class ProprietaryProcessor {
private:
    std::string secretSalt;
    int iterations;

    // Helper function to convert bytes to hex string
    std::string bytesToHex(const unsigned char* data, size_t len) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for(size_t i = 0; i < len; i++) {
            ss << std::setw(2) << static_cast<int>(data[i]);
        }
        return ss.str();
    }

    // Apply SHA256 hash
    std::string sha256(const std::string& input) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, input.c_str(), input.length());
        SHA256_Final(hash, &sha256);
        return bytesToHex(hash, SHA256_DIGEST_LENGTH);
    }

public:
    ProprietaryProcessor() : secretSalt(SECRET_KEY), iterations(ITERATIONS) {}

    // Our secret business logic
    std::string processData(const std::string& input) {
        std::string result = input + secretSalt;
        
        // Apply multiple iterations of hashing (our "secret sauce")
        for(int i = 0; i < iterations; i++) {
            result = sha256(result);
        }
        
        return result;
    }
};

// Global processor instance
ProprietaryProcessor processor;

// HTTP request handler
static enum MHD_Result answer_to_connection(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    
    struct MHD_Response *response;
    enum MHD_Result ret;
    std::string page;

    // Health check endpoint
    if (strcmp(url, "/health") == 0 && strcmp(method, "GET") == 0) {
        time_t now = time(0);
        char* dt = ctime(&now);
        page = "{\"status\":\"healthy\",\"timestamp\":\"" + std::string(dt) + "\"}";
        response = MHD_create_response_from_buffer(page.length(),
                                                   (void*)page.c_str(),
                                                   MHD_RESPMEM_MUST_COPY);
        MHD_add_response_header(response, "Content-Type", "application/json");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Process endpoint
    if (strcmp(url, "/process") == 0 && strcmp(method, "POST") == 0) {
        const char* data = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "data");
        
        if (data == nullptr) {
            page = "{\"error\":\"Missing 'data' parameter\"}";
            response = MHD_create_response_from_buffer(page.length(),
                                                       (void*)page.c_str(),
                                                       MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        } else {
            // Apply our proprietary algorithm
            std::string result = processor.processData(std::string(data));
            page = "{\"input\":\"" + std::string(data) + 
                   "\",\"processed\":\"" + result + 
                   "\",\"algorithm\":\"cpp-proprietary-v1\"}";
            
            response = MHD_create_response_from_buffer(page.length(),
                                                       (void*)page.c_str(),
                                                       MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        }
        
        MHD_destroy_response(response);
        return ret;
    }

    // 404 for other endpoints
    page = "{\"error\":\"Not found\"}";
    response = MHD_create_response_from_buffer(page.length(),
                                               (void*)page.c_str(),
                                               MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(response, "Content-Type", "application/json");
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

int main() {
    struct MHD_Daemon *daemon;
    int port = 8080;

    std::cout << "Starting C++ secure application on port " << port << std::endl;
    std::cout << "Endpoints: /health (GET), /process (POST)" << std::endl;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
    
    if (NULL == daemon) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Server running. Press Enter to stop." << std::endl;
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}

// Made with Bob
