#include <iostream>
#include <vector>
#include <cmath>
#include <json/json.h>
#include <fstream>

// Function to decode a value from the given base
int decode_value(const std::string& value, int base) {
    int decoded_value = 0;
    int power = 0;
    
    // Iterate over each character in the value
    for (int i = value.length() - 1; i >= 0; --i) {
        char ch = value[i];
        int digit;
        
        if (ch >= '0' && ch <= '9') {
            digit = ch - '0';
        } else if (ch >= 'a' && ch <= 'f') {
            digit = ch - 'a' + 10;
        } else if (ch >= 'A' && ch <= 'F') {
            digit = ch - 'A' + 10;
        } else {
            continue;
        }

        decoded_value += digit * std::pow(base, power++);
    }

    return decoded_value;
}

// Lagrange Interpolation function to calculate the constant term 'c'
int lagrange_interpolation(const std::vector<int>& x, const std::vector<int>& y, int k) {
    int c = 0;
    
    // Iterate through each point
    for (int i = 0; i < k; ++i) {
        int term = y[i];
        
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= x[j];
                term /= (x[i] - x[j]);
            }
        }
        
        c += term;
    }

    return c;
}

int main() {
    // Load JSON file
    std::ifstream test_case("testcase1.json");
    Json::Reader reader;
    Json::Value root;
    
    if (!reader.parse(test_case, root)) {
        std::cerr << "Error parsing the JSON file." << std::endl;
        return 1;
    }
    
    // Read n and k
    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();
    
    std::vector<int> x;
    std::vector<int> y;
    
    // Read the roots and decode the y values
    for (const auto& key : root.getMemberNames()) {
        if (key != "keys") {
            int x_value = std::stoi(key);
            int base = root[key]["base"].asInt();
            std::string value = root[key]["value"].asString();
            
            int y_value = decode_value(value, base);
            
            x.push_back(x_value);
            y.push_back(y_value);
        }
    }
    
    // Calculate the constant term using Lagrange Interpolation
    int c = lagrange_interpolation(x, y, k);
    
    std::cout << "The secret (constant term) c is: " << c << std::endl;

    return 0;
}
