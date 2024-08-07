/**
Purpose: Compression and Decompression using Huffman Algorithm
Harshit Sharma
*/

#include <iostream>
#include <functional>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

unsigned char buffer;
int bitCount = 0;
queue<char> output;

struct Node {
    char symbol;
    int freq;
    Node* left, * right;

    Node(char symbol = NULL, int freq = 0) {
        left = right = NULL;
        this->symbol = symbol;
        this->freq = freq;
    }
};

struct compare {
    bool operator()(Node* left, Node* right) {
        return (left->freq > right->freq);
    }
};

void bitWriter(bool bit, ofstream& out) {
    if (bitCount >= 8) {
        out << buffer;
        bitCount = 0;
        buffer = 0;
    }
    buffer = buffer << 1;
    if (bit) {
        buffer++;
    }
    bitCount++;
}

void dec2bit(unsigned char symbol, ofstream& out) {
    unsigned char m = 0;
    unsigned char mask = 128;
    for (int i = 0; i < 8; i++) {
        m = symbol & mask;
        bitWriter(m > 0, out);
        mask = mask >> 1;
    }
}

void bitReader(unsigned int symbol) {
    stack<char> temp;
    for (int i = 0; i < 8; ++i) {
        temp.push(symbol % 2 + '0');
        symbol >>= 1;
    }
    while (!temp.empty()) {
        output.push(temp.top());
        temp.pop();
    }
}

char getBit() {
    if (output.empty()) return 0;
    char ret = output.front();
    output.pop();
    return ret;
}

char getByte() {
    unsigned char ret = 0;
    unsigned char temp = 0;
    for (int i = 0; i < 8; i++) {
        temp = getBit();
        ret = ret << 1;
        if (temp != '0') {
            ret++;
        }
    }
    return ret;
}

void StoreTree(Node* node, ofstream& out) {
    if (!node) return;

    if (!node->left && !node->right) {
        bitWriter(1, out);
        dec2bit(node->symbol, out);
    }
    else {
        bitWriter(0, out);
        StoreTree(node->left, out);
        StoreTree(node->right, out);
    }
}

void getCode(char symbol, Node* root, string& code) {
    if (!root) return;

    if (root->symbol == symbol && code.back() != 'E') {
        code.push_back('E');
        return;
    }

    if (root->left) {
        code.push_back('0');
        getCode(symbol, root->left, code);
        if (code.back() == 'E') return;
        code.pop_back();
    }
    if (root->right) {
        code.push_back('1');
        getCode(symbol, root->right, code);
        if (code.back() == 'E') return;
        code.pop_back();
    }
}

void Compression(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input.is_open()) {
        cerr << "Error opening file for compression: " << filename << endl;
        return;
    }

    int freqTable[256] = { 0 };
    char in;
    while (input.read(&in, 1)) {
        freqTable[static_cast<unsigned char>(in)]++;
    }
    input.clear();
    input.seekg(0);

    priority_queue<Node*, vector<Node*>, compare> minHeap;
    for (int i = 0; i < 256; i++) {
        if (freqTable[i]) {
            minHeap.push(new Node(i, freqTable[i]));
        }
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        Node* alpha = new Node;
        alpha->freq = left->freq + right->freq;
        alpha->left = left;
        alpha->right = right;
        minHeap.push(alpha);
    }

    string outputFilename = filename.substr(0, filename.find_last_of('.')) + ".mcf";
    ofstream out(outputFilename, ios::binary);
    if (!out.is_open()) {
        cerr << "Error opening file for writing: " << outputFilename << endl;
        return;
    }

    StoreTree(minHeap.top(), out);
    while (bitCount != 8) {
        buffer = buffer << 1;
        bitCount++;
    }
    out << buffer;
    bitCount = 0;

    char inp;
    string code;
    while (input.get(inp)) {
        getCode(inp, minHeap.top(), code);
        code.pop_back();
        for (char c : code) {
            bitWriter(c - '0', out);
        }
        code.clear();
    }
    getCode(1, minHeap.top(), code);
    while (bitCount != 8) {
        buffer = buffer << 1;
        bitCount++;
    }
    out << buffer;

    cout << "Compression successful: " << outputFilename << endl;
}

Node* treeReader() {
    if (output.empty()) return nullptr;
    char bit = getBit();
    Node* root = new Node;
    if (bit == '0') {
        root->left = treeReader();
        root->right = treeReader();
    }
    else if (bit == '1') {
        root->symbol = getByte();
        root->left = root->right = nullptr;
    }
    return root;
}

char deCode(Node* root, ifstream& inp) {
    if (!root) return '\0';
    if (!root->left && !root->right) {
        return root->symbol;
    }
    char bit;
    while (output.empty() && inp.get(bit)) {
        bitReader(bit);
    }
    bit = getBit();
    if (bit == '0' && root->left) {
        return deCode(root->left, inp);
    }
    else if (bit == '1' && root->right) {
        return deCode(root->right, inp);
    }
    return '\0';
}

void Decompression(const string& filename) {
    ifstream input(filename, ios::binary);
    if (!input.is_open()) {
        cerr << "Error opening file for decompression: " << filename << endl;
        return;
    }

    char sym;
    while (input.get(sym) && sym != ' ') {
        bitReader(sym);
    }

    Node* root = treeReader();
    string outputFilename = filename.substr(0, filename.find_last_of('.')) + ".txt";
    ofstream out(outputFilename);
    if (!out.is_open()) {
        cerr << "Error opening file for writing: " << outputFilename << endl;
        return;
    }

    while (input.get(sym)) {
        bitReader(sym);
        char decodedChar = deCode(root, input);
        if (decodedChar != 1) {  // Check for pseudo EOF
            out << decodedChar;
        }
        else {
            break;
        }
    }

    while (!output.empty()) {
        char decodedChar = deCode(root, input);
        if (decodedChar != 1) {  // Check for pseudo EOF
            out << decodedChar;
        }
        else {
            break;
        }
    }

    cout << "Decompression successful: " << outputFilename << endl;
}

int main() {
    string command;
    while (true) {
        cout << "Enter command (compress <filename>, decompress <filename>, exit): ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        size_t spacePos = command.find(' ');
        if (spacePos == string::npos) {
            cerr << "Invalid command format." << endl;
            continue;
        }

        string operation = command.substr(0, spacePos);
        string filename = command.substr(spacePos + 1);

        if (operation == "compress") {
            Compression(filename);
        }
        else if (operation == "decompress") {
            Decompression(filename);
        }
        else {
            cerr << "Unknown command: " << operation << endl;
        }
    }

    return 0;
}
