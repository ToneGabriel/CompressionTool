#include "x_Huffman_Tree.h"


_Huffman_Node::_Huffman_Node(symbol_t byte, size_t frequency)
    : _Byte(byte), _Frequency(frequency), _Left(nullptr), _Right(nullptr) { /*Empty*/ }

_Huffman_Node::_Huffman_Node(size_t frequency, _Huffman_Node* left, _Huffman_Node* right)
    : _Byte(0), _Frequency(frequency), _Left(left), _Right(right) { /*Empty*/ }

_Huffman_Node::~_Huffman_Node()
{
    // TODO
}

bool _Huffman_Node::operator()(_Huffman_Node* left, _Huffman_Node* right) const noexcept
{
    return left->_Frequency < right->_Frequency;
}