#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/sha.h>

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
        std::string toHash = ss.str();

        unsigned char hashBytes[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)toHash.c_str(), toHash.size(), hashBytes);

        std::stringstream hashString;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hashString << std::hex << std::setw(2) << std::setfill('0') << (int)hashBytes[i];
        }
        return hashString.str();
    }

    void setHash(std::string newHash) {
        hash = newHash;
    }

    void setPreviousHash(std::string prevHash) {
        previousHash = prevHash;
    }

    void setNonce(int newNonce) {
        nonce = newNonce;
    }

    std::string getHash() {
        return hash;
    }

    std::string getPreviousHash() {
        return previousHash;
    }

    int getNonce() {
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

public:
    BCNode() {
        Block genesisBlock;
        blockchain.push_back(genesisBlock);
    }

    void addBlock(Block b) {
        b.setPreviousHash(blockchain.back().getHash());
        mineBlock(b, 5);
        blockchain.push_back(b);
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
};

int main() {
    BCNode node;
    Block block1("Block 1 Data");
    Block block2("Block 2 Data");

    node.addBlock(block1);
    node.addBlock(block2);

    std::cout << node.toString() << std::endl;
    std::cout << "Is blockchain valid? " << (node.isChainValid() ? "Yes" : "No") << std::endl;

    return 0;
}