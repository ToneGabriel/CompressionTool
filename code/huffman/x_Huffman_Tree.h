#pragma once

#include <unordered_map>
#include <queue>

#include "_Core.h"



using symbol_t = char;


DETAIL_BEGIN

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


struct _Huffman_Node_Greater_Compare
{
    bool operator()(const _Huffman_Node* const left, const _Huffman_Node* const right);
};  // END _Huffman_Node_Greater_Compare


class _Huffman_Tree
{
private:

    using _Node     = _Huffman_Node;
    using _Compare  = _Huffman_Node_Greater_Compare;

private:
    _Node* _root = nullptr;

public:
    // Constructors & Operators

    _Huffman_Tree() = default;
    ~_Huffman_Tree();

    _Huffman_Tree(const _Huffman_Tree& other);
    _Huffman_Tree(_Huffman_Tree&& other) noexcept;

    _Huffman_Tree& operator=(const _Huffman_Tree& other);
    _Huffman_Tree& operator=(_Huffman_Tree&& other) noexcept;

public:
    // Main functions

    void build(const std::unordered_map<symbol_t, size_t>& frequencies);
    void clear();
    void print() const;

private:
    // Helpers

    void _copy(const _Huffman_Tree& other);
    void _move(_Huffman_Tree&& other) noexcept;

    _Node* _copy_subtree(_Node* subroot) const;
    void _delete_subtree(_Node* subroot);
    void _print_subtree(const size_t ident, const _Node* const subroot, const std::string& rlFlag) const;
};  // END _Huffman_Tree

DETAIL_END