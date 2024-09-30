#include "Huffman_Compressor.h"

namespace fs = std::filesystem;

// HELP: Declarations
// ==================================================================================================================================================
// ==================================================================================================================================================

struct _Huffman_Node
{
    symbol_t        _Symbol;
    size_t          _Frequency;
    _Huffman_Node*  _Left;
    _Huffman_Node*  _Right;

    _Huffman_Node() = delete;
    ~_Huffman_Node() = default;

    _Huffman_Node(symbol_t symbol, size_t frequency, _Huffman_Node* left = nullptr, _Huffman_Node* right = nullptr);

    _Huffman_Node(const _Huffman_Node& other) = delete;
    _Huffman_Node& operator=(const _Huffman_Node& other) = delete;
};  // END _Huffman_Node


struct _Huffman_NodePtr_Greater_Compare
{
    bool operator()(const _Huffman_Node* const left, const _Huffman_Node* const right);
};  // END _Huffman_NodePtr_Greater_Compare


_Huffman_Node* _build_huffman_tree(const std::unordered_map<symbol_t, size_t>& frequencies);
void _delete_tree(_Huffman_Node* subroot);
void _print_tree(const size_t ident, const _Huffman_Node* const subroot, const std::string& property);


std::unordered_map<symbol_t, size_t> _get_symbol_frequencies(const std::string& filename);
void _generate_huffman_codes_rec(const _Huffman_Node* const subroot, const std::string& code, std::unordered_map<symbol_t, std::string>& codes);
std::unordered_map<symbol_t, std::string> _generate_huffman_codes(const std::unordered_map<symbol_t, size_t>& frequencies);


void _wirte_compressed_file(const std::string& filename, const std::unordered_map<symbol_t, std::string>& codeMap);
void _wirte_decompressed_file(const std::string& binFilename, const std::unordered_map<symbol_t, size_t>& frequencyMap);


// MAIN: Definitions
// ==================================================================================================================================================
// ==================================================================================================================================================

HUFFMAN_BEGIN

void compress(const std::string& filename)
{
    auto frequencyMap = _get_symbol_frequencies(filename);

    auto codeMap = _generate_huffman_codes(frequencyMap);

    _wirte_compressed_file(filename, codeMap);
}

void decompress(const std::string& filename)
{
    // _wirte_decompressed_file("H:\\Programare\\C++\\CompressionTool\\input.bin", frequencyMap);
}

HUFFMAN_END



// HELP: Definitions
// ==================================================================================================================================================
// ==================================================================================================================================================

_Huffman_Node::_Huffman_Node(symbol_t symbol, size_t frequency, _Huffman_Node* left, _Huffman_Node* right)
    :   _Symbol(symbol),
        _Frequency(frequency),
        _Left(left),
        _Right(right)
{
    /*Empty*/
}


bool _Huffman_NodePtr_Greater_Compare::operator()(const _Huffman_Node* const left, const _Huffman_Node* const right)
{
    return left->_Frequency > right->_Frequency;
}


_Huffman_Node* _build_huffman_tree(const std::unordered_map<symbol_t, size_t>& frequencies)
{
    _ASSERT(!frequencies.empty(), "No frequency data!");

    _Huffman_Node* root = nullptr;

    // Create leaf nodes for each symbol and push them to the priority queue
    std::priority_queue<_Huffman_Node*,
                        std::vector<_Huffman_Node*>,
                        _Huffman_NodePtr_Greater_Compare> prioq;

    for (const auto& [symbol, frequency] : frequencies)
        prioq.push(new _Huffman_Node(symbol, frequency));

    while (prioq.size() > 1)    // Last elem will be root
    {
        // Remove two nodes of lowest frequency
        _Huffman_Node* leftNode = prioq.top();
        prioq.pop();

        _Huffman_Node* rightNode = prioq.top();
        prioq.pop();
        
        // Create a new internal subroot with these two nodes as children
        prioq.push(new _Huffman_Node(   static_cast<symbol_t>(0),
                                        leftNode->_Frequency + rightNode->_Frequency,
                                        leftNode,
                                        rightNode));
    }

    // Last elem is root
    root = prioq.top();
    prioq.pop();

    return root;
}

void _delete_tree(_Huffman_Node* subroot)
{
    if (subroot == nullptr)
        return;

    _delete_tree(subroot->_Left);
    _delete_tree(subroot->_Right);

    delete subroot;
}

void _print_tree(   const size_t ident,
                    const _Huffman_Node* const subroot,
                    const std::string& property)
{
    if (subroot != nullptr)
        std::cout   << std::string("").append(ident, '\t')
                    << subroot->_Symbol
                    << " [" << subroot->_Frequency << " "<< property << "]\n";

    if (subroot->_Left != nullptr)
        _print_tree(ident + 1, subroot->_Left, "LEFT");

    if (subroot->_Right != nullptr)
        _print_tree(ident + 1, subroot->_Right, "RIGHT");
}

std::unordered_map<symbol_t, size_t> _get_symbol_frequencies(const std::string& filename)
{
    _ASSERT(fs::exists(filename), "File not found!");

    std::unordered_map<symbol_t, size_t> ret;

    std::ifstream file(filename, std::ios::binary);

    symbol_t symbol;

    while (file.get(symbol))
        ++ret[symbol];

    file.close();

    return ret;
}

void _generate_huffman_codes_rec(   const _Huffman_Node* const subroot,
                                    const std::string& code,
                                    std::unordered_map<symbol_t, std::string>& codes)
{
    if (subroot == nullptr)
        return;

    // If it's a leaf node, store the code
    if (subroot->_Left == nullptr && subroot->_Right == nullptr)
    {
        codes[subroot->_Symbol] = code;
        return;
    }

    // Traverse left subtree, append '0'
    _generate_huffman_codes_rec(subroot->_Left, code + ZERO_CHR, codes);

    // Traverse right subtree, append '1'
    _generate_huffman_codes_rec(subroot->_Right, code + ONE_CHR, codes);
}

std::unordered_map<symbol_t, std::string> _generate_huffman_codes(const std::unordered_map<symbol_t, size_t>& frequencies)
{
    std::unordered_map<symbol_t, std::string> ret;

    _Huffman_Node* root = _build_huffman_tree(frequencies);

    // _print_tree(0, root, "ROOT");

    _generate_huffman_codes_rec(root, "", ret);

    _delete_tree(root);

    return ret;
}

void _wirte_compressed_file(const std::string& filename, const std::unordered_map<symbol_t, std::string>& codeMap)
{
    // Check original and create new .bin file with same name
    _ASSERT(fs::exists(filename), "File not found!");

    std::string newBinFilename = fs::path(filename).replace_extension(BIN_EXTENSION).string();

    std::ifstream originalFile(filename, std::ios::binary);
    std::ofstream newBinFile(newBinFilename, std::ios::binary);

    // Write data
    std::string buffer;
    symbol_t symbol;

    while (originalFile.get(symbol))
    {
        buffer.append(codeMap.at(symbol));

        while (buffer.size() >= SYMBOL_BIT)
        {
            // Take necessary bits from buffer and transform them into bitset
            std::bitset<SYMBOL_BIT> bits(buffer, 0, SYMBOL_BIT);

            // Remaining bits will be used in next iterations
            buffer.erase(0, SYMBOL_BIT);

            // Transform bits from bitset into a symbol and write it
            symbol_t bitsToSymbol = static_cast<symbol_t>(bits.to_ulong());
            newBinFile.put(bitsToSymbol);
        }
    }

    // If there are leftover bits in the buffer, pad with 0s and write them as the last byte
    if (!buffer.empty())
    {
        // Pad the remaining bits to form a full byte
        buffer.append(SYMBOL_BIT - buffer.size(), ZERO_CHR);
        std::bitset<SYMBOL_BIT> bits(buffer);

        symbol_t bitsToSymbol = static_cast<symbol_t>(bits.to_ulong());
        newBinFile.put(bitsToSymbol);
    }

    // Close files
    originalFile.close();
    newBinFile.close();
}

void _wirte_decompressed_file(const std::string& binFilename, const std::unordered_map<symbol_t, size_t>& frequencyMap)
{
    // Check .bin and create new txt file with same name
    _ASSERT(fs::exists(binFilename), "File not found!");

    std::string newOriginalFilename = fs::path(binFilename).replace_extension(".txtd").string();

    std::ifstream binFile(binFilename, std::ios::binary);
    std::ofstream originalFile(newOriginalFilename, std::ios::binary);

    // Build Huffman tree from frequencies
    _Huffman_Node* root         = _build_huffman_tree(frequencyMap);
    _Huffman_Node* currentNode  = root;

    std::string buffer;
    symbol_t symbol;

    while (binFile.get(symbol))
    {
        std::bitset<SYMBOL_BIT> bits(symbol);
        buffer = std::move(bits.to_string());

        for (symbol_t digit : buffer)
        {
            if (digit == ZERO_CHR)
                currentNode = currentNode->_Left;
            else
                currentNode = currentNode->_Right;

            if (currentNode->_Left == nullptr && currentNode->_Right == nullptr)
            {
                originalFile.put(currentNode->_Symbol);     // Write the character to the output file
                currentNode = root;                         // Reset to root for next character
            }
            else
            {
                // do nothing - currentNode remains the same
            }
        }
    }

    _delete_tree(root);

    binFile.close();
    originalFile.close();
}