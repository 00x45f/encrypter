#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cryptopp/fernet.h>
#include <cryptopp/osrng.h>

int encryptFile(const std::string &file_name, const CryptoPP::SecByteBlock &key) {
    try {
        std::ifstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "File not found: " << file_name << std::endl;
            return 1;
        }

        std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        CryptoPP::Fernet::Encryption fernet(key);
        std::string encrypted_data;
        fernet.ProcessData(encrypted_data, (const byte*)data.c_str(), data.size());

        remove(file_name.c_str());

        std::ofstream encrypted_file("example_encrypted.txt", std::ios::binary);
        encrypted_file.write(encrypted_data.c_str(), encrypted_data.size());

        std::cout << "File encrypted successfully." << std::endl;

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred during encryption: " << e.what() << std::endl;
        return 1;
    }
}

int main() {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock key(CryptoPP::Fernet::KEYLENGTH);
    prng.GenerateBlock(key, key.size());

    int result = encryptFile("example.txt", key);

    if (result == 0) {
        std::ofstream logFile("encryption.log", std::ios::app);
        logFile << "Encryption successful." << std::endl;
    } else {
        std::cerr << "Encryption failed." << std::endl;
        std::ofstream logFile("encryption.log", std::ios::app);
        logFile << "Encryption failed." << std::endl;
    }

    return 0;
}
