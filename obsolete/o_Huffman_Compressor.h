namespace fs = std::filesystem;

void generateCodes(_Huffman_Node* root, const std::string& code, std::unordered_map<unsigned char, std::string>& huffmanCode) {
    if (!root) return;

    if (!root->_Left && !root->_Right) {
        huffmanCode[root->_Symbol] = code;
    }

    generateCodes(root->_Left, code + "0", huffmanCode);
    generateCodes(root->_Right, code + "1", huffmanCode);
}

void freeTree(_Huffman_Node* root) {
    if (!root) return;
    freeTree(root->_Left);
    freeTree(root->_Right);
    delete root;
}

void writeEncodedToFile(const std::string& encodedStr, std::ofstream& outFile) {
    for (size_t i = 0; i < encodedStr.size(); i += 8) {
        std::string byte = encodedStr.substr(i, 8);
        std::bitset<8> bits(byte);
        outFile.put(static_cast<unsigned char>(bits.to_ulong()));
    }
}

std::string readEncodedFromFile(std::ifstream& inFile) {
    std::string encodedStr = "";
    char byte;

    while (inFile.get(byte)) {
        std::bitset<8> bits(byte);
        encodedStr += bits.to_string();
    }

    return encodedStr;
}

void writeTreeToFile(_Huffman_Node* root, std::ofstream& outFile) {
    if (!root) {
        outFile.put(0); // Null marker for empty node
        return;
    }
    outFile.put(1); // Marker for internal node
    outFile.put(root->_Symbol);
    writeTreeToFile(root->_Left, outFile);
    writeTreeToFile(root->_Right, outFile);
}

_Huffman_Node* readTreeFromFile(std::ifstream& inFile) {
    char marker;
    inFile.get(marker);
    if (marker == 0) {
        return nullptr;
    }
    char byte;
    inFile.get(reinterpret_cast<char&>(byte));
    _Huffman_Node* node = new _Huffman_Node(byte, 0);
    node->_Left = readTreeFromFile(inFile);
    node->_Right = readTreeFromFile(inFile);
    return node;
}

void huffmanCompressFolder(const std::string& folderPath, const std::string& compressedFileName) {
    std::vector<unsigned char> allFileData;
    std::vector<std::pair<std::string, size_t>> fileMetadata;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path(), std::ios::binary);
            if (!file.is_open()) {
                std::cout << "Error opening file: " << entry.path() << std::endl;
                continue;
            }

            std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            allFileData.insert(allFileData.end(), fileData.begin(), fileData.end());

            fileMetadata.push_back({ entry.path().filename().string(), fileData.size() });
        }
    }

    std::unordered_map<unsigned char, int> freqMap;
    for (unsigned char byte : allFileData) {
        freqMap[byte]++;
    }

    std::priority_queue<_Huffman_Node*> pq;

    for (auto pair : freqMap) {
        pq.push(new _Huffman_Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        _Huffman_Node* _Left = pq.top(); pq.pop();
        _Huffman_Node* _Right = pq.top(); pq.pop();

        _Huffman_Node* newNode = new _Huffman_Node(_Left->_Frequency + _Right->_Frequency, _Left, _Right);
        pq.push(newNode);
    }

    _Huffman_Node* root = pq.top();

    std::unordered_map<unsigned char, std::string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    std::string encodedStr = "";
    for (unsigned char byte : allFileData) {
        encodedStr += huffmanCode[byte];
    }

    std::ofstream compressedFile(compressedFileName, std::ios::binary);
    if (!compressedFile.is_open()) {
        std::cout << "Error: Unable to create compressed file " << compressedFileName << std::endl;
        return;
    }

    for (const auto& [filename, size] : fileMetadata) {
        compressedFile << filename << '\0';
        compressedFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    }

    writeTreeToFile(root, compressedFile);
    writeEncodedToFile(encodedStr, compressedFile);

    compressedFile.close();
    freeTree(root);

    std::cout << "Folder compression complete." << std::endl;
}

void huffmanDecompressFolder(const std::string& compressedFileName, const std::string& outputFolderPath) {
    std::ifstream compressedFile(compressedFileName, std::ios::binary);
    if (!compressedFile.is_open()) {
        std::cout << "Error: Unable to open compressed file " << compressedFileName << std::endl;
        return;
    }

    std::vector<std::pair<std::string, size_t>> fileMetadata;
    while (true) {
        std::string filename;
        std::getline(compressedFile, filename, '\0');
        if (filename.empty()) break;

        size_t fileSize;
        compressedFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

        fileMetadata.push_back({ filename, fileSize });
    }

    _Huffman_Node* root = readTreeFromFile(compressedFile);
    std::string encodedStr = readEncodedFromFile(compressedFile);
    compressedFile.close();

    std::string decodedStr = "";
    _Huffman_Node* current = root;
    for (char bit : encodedStr) {
        current = bit == '0' ? current->_Left : current->_Right;
        if (!current->_Left && !current->_Right) {
            decodedStr += current->_Symbol;
            current = root;
        }
    }

    size_t index = 0;
    for (const auto& [filename, size] : fileMetadata) {
        std::ofstream outFile(outputFolderPath + "/" + filename, std::ios::binary);
        if (!outFile.is_open()) {
            std::cout << "Error: Unable to create output file " << outputFolderPath + "/" + filename << std::endl;
            return;
        }
        outFile.write(decodedStr.data() + index, size);
        index += size;
    }

    std::cout << "Decompression complete, files restored to " << outputFolderPath << std::endl;
}