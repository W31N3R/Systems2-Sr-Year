#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/sha.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Block {
public:
    std::string data;
    long timestamp;
    int nonce;
    std::string hash;
    std::string previousHash;

    Block(std::string data) : data(data), nonce(0) {
        timestamp = std::time(0);
        hash = calculateBlockHash();
    }

    Block() : data("Genesis Block"), nonce(0) {
        timestamp = std::time(0);
        hash = calculateBlockHash();
    }

    std::string calculateBlockHash() {
        std::stringstream ss;
        ss << data << timestamp << nonce << previousHash;

        unsigned char hashBytes[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)ss.str().c_str(), ss.str().size(), hashBytes);

        std::stringstream hashString;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hashString << std::hex << std::setw(2) << std::setfill('0') << (int)hashBytes[i];
        }
        return hashString.str();
    }

    void setHash(const std::string& newHash) {
        hash = newHash;
    }

    void setPreviousHash(const std::string& prevHash) {
        previousHash = prevHash;
    }

    void setNonce(int newNonce) {
        nonce = newNonce;
    }

    std::string getHash() const {
        return hash;
    }

    std::string getPreviousHash() const {
        return previousHash;
    }

    int getNonce() const {
        return nonce;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "Block[data=" << data << ", timestamp=" << timestamp << ", nonce=" << nonce
           << ", hash=" << hash << ", previousHash=" << previousHash << "]";
        return ss.str();
    }
};

class BCNode {
private:
    std::vector<Block> blockchain;
    std::vector<int> connections;
    std::mutex mtx;
    int port;

public:
    BCNode(int port, const std::vector<int>& remotePorts) : port(port) {
        Block genesisBlock;
        blockchain.push_back(genesisBlock);

        for (int remotePort : remotePorts) {
            int sockfd = connectToNode(remotePort);
            if (sockfd != -1) {
                connections.push_back(sockfd);
                std::thread(&BCNode::readHandler, this, sockfd).detach();
            }
        }

        std::thread(&BCNode::connectionHandler, this).detach();
    }

    void addBlock(Block b) {
        std::lock_guard<std::mutex> lock(mtx);
        b.setPreviousHash(blockchain.back().getHash());
        mineBlock(b, 5);
        blockchain.push_back(b);
        broadcastBlock(b);
    }

    void mineBlock(Block &b, int prefixZeros) {
        std::string prefix(prefixZeros, '0');
        while (b.calculateBlockHash().substr(0, prefixZeros) != prefix) {
            b.setNonce(b.getNonce() + 1);
            b.setHash(b.calculateBlockHash());
        }
    }

    bool isChainValid() {
        for (size_t i = 1; i < blockchain.size(); ++i) {
            Block currentBlock = blockchain[i];
            Block previousBlock = blockchain[i - 1];

            if (currentBlock.getHash() != currentBlock.calculateBlockHash()) {
                return false;
            }

            if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
                return false;
            }

            std::string prefix(5, '0');
            if (currentBlock.getHash().substr(0, 5) != prefix) {
                return false;
            }
        }
        return true;
    }

    std::string toString() {
        std::stringstream ss;
        for (const auto &block : blockchain) {
            ss << block.toString() << "\n";
        }
        return ss.str();
    }

private:
    int connectToNode(int remotePort) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(remotePort);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Error connecting to node on port " << remotePort << "\n";
            close(sockfd);
            return -1;
        }

        return sockfd;
    }

    void connectionHandler() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            std::cerr << "Socket failed\n";
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            std::cerr << "setsockopt\n";
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed\n";
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen\n";
            exit(EXIT_FAILURE);
        }

        while (true) {
            int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            if (new_socket < 0) {
                std::cerr << "Accept\n";
                exit(EXIT_FAILURE);
            }
            connections.push_back(new_socket);
            std::thread(&BCNode::readHandler, this, new_socket).detach();
        }
    }

    void readHandler(int sockfd) {
        char buffer[1024] = {0};
        while (true) {
            int valread = read(sockfd, buffer, 1024);
            if (valread <= 0) {
                close(sockfd);
                break;
            }
            std::string blockData(buffer, valread);
            Block b = deserializeBlock(blockData);
            if (isValidBlock(b)) {
                addBlock(b);
                broadcastBlock(b);
            }
        }
    }

    void broadcastBlock(const Block& b) {
        std::string blockData = serializeBlock(b);
        for (int sockfd : connections) {
            send(sockfd, blockData.c_str(), blockData.size(), 0);
        }
    }

    bool isValidBlock(const Block& b) {
        if (b.getHash() != b.calculateBlockHash()) {
            return false;
        }

        if (b.getPreviousHash() != blockchain.back().getHash()) {
            return false;
        }

        std::string prefix(5, '0');
        if (b.getHash().substr(0, 5) != prefix) {
            return false;
        }

        return true;
    }

    std::string serializeBlock(const Block& b) {
        std::stringstream ss;
        ss << b.data << "|" << b.timestamp << "|" << b.nonce << "|" << b.hash << "|" << b.previousHash;
        return ss.str();
    }

    Block deserializeBlock(const std::string& blockData) {
        std::stringstream ss(blockData);
        std::string data, hash, previousHash;
        long timestamp;
        int nonce;

        std::getline(ss, data, '|');
        ss >> timestamp;
        ss.ignore();
        ss >> nonce;
        ss.ignore();
        std::getline(ss, hash, '|');
        std::getline(ss, previousHash, '|');

        Block b(data);
        b.timestamp = timestamp;
        b.nonce = nonce;
        b.hash = hash;
        b.previousHash = previousHash;

        return b;
    }
};