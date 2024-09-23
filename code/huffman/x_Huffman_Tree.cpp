#include "x_Huffman_Tree.h"


DETAIL_BEGIN

_Huffman_Node::_Huffman_Node(symbol_t symbol, size_t frequency, _Huffman_Node* left, _Huffman_Node* right)
    :   _Symbol(symbol),
        _Frequency(frequency),
        _Left(left),
        _Right(right)
{
    /*Empty*/
}


bool _Huffman_Node_Greater_Compare::operator()(const _Huffman_Node* const left, const _Huffman_Node* const right)
{
    return left->_Frequency > right->_Frequency;
}


_Huffman_Tree::~_Huffman_Tree()
{
    clear();
}

_Huffman_Tree::_Huffman_Tree(const _Huffman_Tree& other)
{
    _copy(other);
}

_Huffman_Tree::_Huffman_Tree(_Huffman_Tree&& other) noexcept
{
    _move(std::move(other));
}

_Huffman_Tree& _Huffman_Tree::operator=(const _Huffman_Tree& other)
{
    if (_root != other._root)
    {
        clear();
        _copy(other);
    }

    return *this;
}

_Huffman_Tree& _Huffman_Tree::operator=(_Huffman_Tree&& other) noexcept
{
    if (_root != other._root)
    {
        clear();
        _move(std::move(other));
    }

    return *this;
}

void _Huffman_Tree::build(const std::unordered_map<symbol_t, size_t>& frequencies)
{
    if (_root != nullptr)
        clear();

    if (frequencies.empty())
        return;

    // Create leaf nodes for each symbol and push them to the priority queue
    std::priority_queue<_Node*, std::vector<_Node*>, _Compare> prioq;

    for (const auto& [symbol, frequency] : frequencies)
        prioq.push(new _Node(symbol, frequency));

    while (prioq.size() > 1)    // Last elem will be root
    {
        // Remove two nodes of lowest frequency
        _Node* leftNode = prioq.top();
        prioq.pop();

        _Node* rightNode = prioq.top();
        prioq.pop();
        
        // Create a new internal node with these two nodes as children
        prioq.push(new _Node(   static_cast<symbol_t>(0),
                                leftNode->_Frequency + rightNode->_Frequency,
                                leftNode,
                                rightNode));
    }

    // Last elem is root
    _root = prioq.top();
    prioq.pop();
}

void _Huffman_Tree::clear()
{
    _delete_subtree(_root);
    _root = nullptr;
}

void _Huffman_Tree::print() const
{
    _print_subtree(0, _root, "ROOT");
}

void _Huffman_Tree::_copy(const _Huffman_Tree& other)
{
    _root = _copy_subtree(other._root);
}

void _Huffman_Tree::_move(_Huffman_Tree&& other) noexcept
{
    std::swap(_root, other._root);
}

_Huffman_Tree::_Node* _Huffman_Tree::_copy_subtree(_Node* subroot) const
{
    if (subroot == nullptr)
        return subroot;

    _Node* newNode  = new _Node(subroot->_Symbol, subroot->_Frequency);
    newNode->_Left  = _copy_subtree(subroot->_Left);
    newNode->_Right = _copy_subtree(subroot->_Right);

    return newNode;
}

void _Huffman_Tree::_delete_subtree(_Node* subroot)
{
    if (subroot == nullptr)
        return;

    _delete_subtree(subroot->_Left);
    _delete_subtree(subroot->_Right);

    delete subroot;
}

void _Huffman_Tree::_print_subtree(const size_t ident, const _Node* const subroot, const std::string& rlFlag) const
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

DETAIL_END