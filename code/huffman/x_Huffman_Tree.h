#pragma once

#include <unordered_map>

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



class _Huffman_Tree
{
private:

    using _Node = _Huffman_Node;

private:
    _Node* _root = nullptr;

public:

    _Huffman_Tree() = default;
    ~_Huffman_Tree() = default;

public:

    void build(const std::unordered_map<symbol_t, size_t>& frequencies);

    void clear()
    {
        _delete_subtree(_root);
    }

private:

    void _delete_subtree(_Node* subroot)
    {
        if (nullptr == subroot)
            return;

        _delete_subtree(subroot->_Left);
        _delete_subtree(subroot->_Right);

        delete subroot;
    }

};  // END _Huffman_Tree