#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <conio.h>

struct PasswordEntry {
    std::string service;
    std::string username;
    std::string password;
};

std::vector<PasswordEntry> passwords;
const std::string FILENAME = "passwords.txt";

// Simple XOR encryption (not secure - for demo only)
std::string encrypt(const std::string& text) {
    std::string result = text;
    for (int i = 0; i < result.length(); i++) {
        result[i] = result[i] ^ 42;
    }
    return result;
}

std::string decrypt(const std::string& text) {
    return encrypt(text); // XOR is symmetric
}

void savePasswords() {
    std::ofstream file(FILENAME);
    if (!file.is_open()) {
        std::cerr << "Error saving passwords!" << std::endl;
        return;
    }
    
    for (const auto& entry : passwords) {
        file << entry.service << "|" 
             << entry.username << "|" 
             << encrypt(entry.password) << "\n";
    }
    file.close();
    std::cout << "Passwords saved!" << std::endl;
}

void loadPasswords() {
    std::ifstream file(FILENAME);
    if (!file.is_open()) {
        return; // File doesn't exist yet
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            PasswordEntry entry;
            entry.service = line.substr(0, pos1);
            entry.username = line.substr(pos1 + 1, pos2 - pos1 - 1);
            entry.password = decrypt(line.substr(pos2 + 1));
            passwords.push_back(entry);
        }
    }
    file.close();
}

void addPassword() {
    PasswordEntry entry;
    std::cout << "\n=== Add New Password ===" << std::endl;
    std::cout << "Service/Website: ";
    std::getline(std::cin, entry.service);
    std::cout << "Username/Email: ";
    std::getline(std::cin, entry.username);
    std::cout << "Password: ";
    std::getline(std::cin, entry.password);
    
    passwords.push_back(entry);
    savePasswords();
    std::cout << "Password added successfully!" << std::endl;
}

void viewPasswords() {
    if (passwords.empty()) {
        std::cout << "\nNo passwords stored yet." << std::endl;
        return;
    }
    
    std::cout << "\n=== Stored Passwords ===" << std::endl;
    for (size_t i = 0; i < passwords.size(); i++) {
        std::cout << "\n[" << i + 1 << "] Service: " << passwords[i].service << std::endl;
        std::cout << "    Username: " << passwords[i].username << std::endl;
        std::cout << "    Password: " << passwords[i].password << std::endl;
    }
}

void searchPassword() {
    std::string service;
    std::cout << "\n=== Search Password ===" << std::endl;
    std::cout << "Enter service name: ";
    std::getline(std::cin, service);
    
    auto it = std::find_if(passwords.begin(), passwords.end(),
        [&service](const PasswordEntry& e) {
            return e.service == service;
        });
    
    if (it != passwords.end()) {
        std::cout << "\nFound!" << std::endl;
        std::cout << "Service: " << it->service << std::endl;
        std::cout << "Username: " << it->username << std::endl;
        std::cout << "Password: " << it->password << std::endl;
    } else {
        std::cout << "Service not found." << std::endl;
    }
}

void deletePassword() {
    viewPasswords();
    if (passwords.empty()) return;
    
    std::cout << "\nEnter number to delete (0 to cancel): ";
    int num;
    std::cin >> num;
    std::cin.ignore();
    
    if (num > 0 && num <= passwords.size()) {
        passwords.erase(passwords.begin() + num - 1);
        savePasswords();
        std::cout << "Password deleted!" << std::endl;
    } else {
        std::cout << "Invalid selection." << std::endl;
    }
}

void displayMenu() {
    std::cout << "\n===== PASSWORD MANAGER =====" << std::endl;
    std::cout << "1. Add new password" << std::endl;
    std::cout << "2. View all passwords" << std::endl;
    std::cout << "3. Search password" << std::endl;
    std::cout << "4. Delete password" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "Choose option: ";
}

int main() {
    loadPasswords();
    int choice;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                addPassword();
                break;
            case 2:
                viewPasswords();
                break;
            case 3:
                searchPassword();
                break;
            case 4:
                deletePassword();
                break;
            case 5:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Try again." << std::endl;
        }
    }
    
    return 0;
}




