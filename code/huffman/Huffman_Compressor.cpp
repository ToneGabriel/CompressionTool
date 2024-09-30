#include "Huffman_Compressor.h"

namespace fs = std::filesystem;


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
    if (left->_Frequency == right->_Frequency)
        return left->_Symbol > right->_Symbol;  // Tie-breaking: compare by symbol

    return left->_Frequency > right->_Frequency;
}