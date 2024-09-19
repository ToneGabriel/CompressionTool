#pragma once

#include "_Core.h"


struct _Huffman_Node
{
    symbol_t        _Byte;
    size_t          _Frequency;
    _Huffman_Node*  _Left;
    _Huffman_Node*  _Right;

    _Huffman_Node() = delete;
    ~_Huffman_Node();

    _Huffman_Node(symbol_t byte, size_t frequency);
    _Huffman_Node(size_t frequency, _Huffman_Node* left = nullptr, _Huffman_Node* right = nullptr);

    bool operator()(_Huffman_Node* left, _Huffman_Node* right) const noexcept;
};  // END _Huffman_Node