#include "Huffman_Compressor.h"


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
void _delete_subtree(_Huffman_Node* subroot);
void _print_subtree(const size_t ident, const _Huffman_Node* const subroot, const std::string& rlFlag);
void _generate_huffman_codes_rec(const _Huffman_Node* const subroot, const std::string& code, std::unordered_map<symbol_t, std::string>& codes);
std::unordered_map<symbol_t, std::string> _generate_huffman_codes(const std::unordered_map<symbol_t, size_t>& frequencies);



// MAIN: Definitions
// ==================================================================================================================================================
// ==================================================================================================================================================

HUFFMAN_BEGIN

void compress(const std::string& filename)
{
    files::create_empty_file_from(filename, BIN_EXTENSION);

    auto frequencies = files::get_symbol_frequencies(filename);

    auto codes = _generate_huffman_codes(frequencies);
}

void decompress(const std::string& filename)
{

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

void _delete_subtree(_Huffman_Node* subroot)
{
    if (subroot == nullptr)
        return;

    _delete_subtree(subroot->_Left);
    _delete_subtree(subroot->_Right);

    delete subroot;
}

void _print_subtree(const size_t ident,
                    const _Huffman_Node* const subroot,
                    const std::string& rlFlag)
{
    std::string str;
    str.append(ident, '\t');

    if (subroot != nullptr)
        std::cout << str << subroot->_Frequency << " [" << subroot->_Symbol << " " << rlFlag << "]\n";

    if (subroot->_Left != nullptr)
        _print_subtree(ident + 1, subroot->_Left, "LEFT");

    if (subroot->_Right != nullptr)
        _print_subtree(ident + 1, subroot->_Right, "RIGHT");
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
    _generate_huffman_codes_rec(subroot->_Left, code + "0", codes);

    // Traverse right subtree, append '1'
    _generate_huffman_codes_rec(subroot->_Right, code + "1", codes);
}

std::unordered_map<symbol_t, std::string> _generate_huffman_codes(const std::unordered_map<symbol_t, size_t>& frequencies)
{
    std::unordered_map<symbol_t, std::string> ret;

    _Huffman_Node* root = _build_huffman_tree(frequencies);

    // _print_subtree(0, root, "ROOT");

    _generate_huffman_codes_rec(root, "", ret);

    _delete_subtree(root);

    return ret;
}