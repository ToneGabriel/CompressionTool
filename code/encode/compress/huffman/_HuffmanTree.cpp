#include "_HuffmanTree.h"


// _HuffmanTreeTraversor ==================================

_HuffmanTreeTraversor::_HuffmanTreeTraversor(_HuffmanNode* node)
    : _current(node) { /*Empty*/ }

const symbol_t& _HuffmanTreeTraversor::get_symbol() const
{
    _ASSERT(_current != nullptr, "Traversor reached end!");

    return _current->_Symbol;
}

const size_t& _HuffmanTreeTraversor::get_frequency() const
{
    _ASSERT(_current != nullptr, "Traversor reached end!");

    return _current->_Frequency;
}

_HuffmanTreeTraversor& _HuffmanTreeTraversor::operator+=(const symbol_t& digit)
{
    _ASSERT(_current != nullptr, "Traversor reached end!");

    switch (digit)
    {
        case SYMBOL_ZERO:
        {
            _current = _current->_Left;
            return *this;
        }
        case SYMBOL_ONE:
        {
            _current = _current->_Right;
            return *this;
        }
        default:
            throw std::invalid_argument("Symbol should be 0 or 1!");
    }
}

bool _HuffmanTreeTraversor::is_leaf() const
{
    _ASSERT(_current != nullptr, "Traversor reached end!");

    return _current->_Left == nullptr && _current->_Right == nullptr;
}


// _HuffmanTree ==================================

_HuffmanTree::~_HuffmanTree()
{
    clear();
}

_HuffmanTree::_HuffmanTree(const _HuffmanTree& other)
{
    _copy_tree(other);
}

_HuffmanTree::_HuffmanTree(_HuffmanTree&& other) noexcept
{
    _move_tree(std::move(other));
}

_HuffmanTree& _HuffmanTree::operator=(const _HuffmanTree& other)
{
    if (_root != other._root)
    {
        clear();
        _copy_tree(other);
    }

    return *this;
}

_HuffmanTree& _HuffmanTree::operator=(_HuffmanTree&& other) noexcept
{
    if (_root != other._root)
    {
        clear();
        _move_tree(std::move(other));
    }

    return *this;
}

_HuffmanTreeTraversor _HuffmanTree::traversor_begin() const
{
    return _HuffmanTreeTraversor(_root);
}

void _HuffmanTree::build(const std::map<symbol_t, size_t>& frequencyMap)
{
    _ASSERT(!frequencyMap.empty(), "No frequency data!");

    if (!empty())
        clear();

    // create leaf nodes for each symbol and push them to the priority queue
    std::priority_queue<_HuffmanNode*,
                        std::vector<_HuffmanNode*>,
                        _HuffmanNodePtrGreaterCompare> prioq;

    _HuffmanNode* leftNode     = nullptr;
    _HuffmanNode* rightNode    = nullptr;

    for (const auto& [symbol, frequency] : frequencyMap)
        prioq.push(new _HuffmanNode(symbol, frequency));

    while (prioq.size() > 1)    // Last elem will be root
    {
        // remove two nodes of lowest frequency
        leftNode = prioq.top();
        prioq.pop();

        rightNode = prioq.top();
        prioq.pop();
        
        // create a new internal subroot with these two nodes as children
        prioq.push(new _HuffmanNode(    static_cast<symbol_t>(0),
                                        leftNode->_Frequency + rightNode->_Frequency,
                                        leftNode,
                                        rightNode));
    }

    // last elem is root
    _root = prioq.top();
    prioq.pop();
}

std::unordered_map<symbol_t, std::string> _HuffmanTree::generate_huffman_codes() const
{
    _check_tree();

    std::unordered_map<symbol_t, std::string> ret;
    _generate_huffman_codes_impl(_root, "", ret);

    return ret;
}

void _HuffmanTree::clear()
{
    _clear_tree_impl(_root);
    _root = nullptr;
}

void _HuffmanTree::print() const
{
    _check_tree();
    _print_tree_impl(0, _root, "ROOT");
}

bool _HuffmanTree::empty() const
{
    return _root == nullptr;
}

void _HuffmanTree::_copy_tree(const _HuffmanTree& other)
{
    _root = _copy_tree_impl(other._root);
}

void _HuffmanTree::_move_tree(_HuffmanTree&& other) noexcept
{
    std::swap(_root, other._root);
}

void _HuffmanTree::_check_tree() const
{
    if (empty())
        throw std::logic_error("Tree not built!");
}

_HuffmanNode* _HuffmanTree::_copy_tree_impl(const _HuffmanNode* const subroot)
{
    if (subroot == nullptr)
        return nullptr;

    _HuffmanNode* newNode = new _HuffmanNode(subroot->_Symbol, subroot->_Frequency);

    newNode->_Left  = _copy_tree_impl(subroot->_Left);
    newNode->_Right = _copy_tree_impl(subroot->_Right);

    return newNode;
}

void _HuffmanTree::_clear_tree_impl(_HuffmanNode* subroot)
{
    if (subroot == nullptr)
        return;

    _clear_tree_impl(subroot->_Left);
    _clear_tree_impl(subroot->_Right);

    delete subroot;
}

void _HuffmanTree::_print_tree_impl(const size_t ident,
                                    const _HuffmanNode* const subroot,
                                    const std::string& property)
{
    if (subroot != nullptr)
        std::cout   << std::string("").append(ident, '\t')
                    << subroot->_Symbol
                    << " [" << subroot->_Frequency << " "<< property << "]\n";

    if (subroot->_Left != nullptr)
        _print_tree_impl(ident + 1, subroot->_Left, "LEFT");

    if (subroot->_Right != nullptr)
        _print_tree_impl(ident + 1, subroot->_Right, "RIGHT");
}

void _HuffmanTree::_generate_huffman_codes_impl(const _HuffmanNode* const subroot,
                                                const std::string& code,
                                                std::unordered_map<symbol_t, std::string>& codes)
{
    if (subroot == nullptr)
        return;

    // if it's a leaf node, store the code
    if (subroot->_Left == nullptr && subroot->_Right == nullptr)
    {
        codes[subroot->_Symbol] = code;
        return;
    }

    // traverse left subtree, append '0'
    _generate_huffman_codes_impl(subroot->_Left, code + SYMBOL_ZERO, codes);

    // traverse right subtree, append '1'
    _generate_huffman_codes_impl(subroot->_Right, code + SYMBOL_ONE, codes);
}