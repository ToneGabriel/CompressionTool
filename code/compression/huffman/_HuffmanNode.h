#pragma once

#include "_Core.h"

struct _HuffmanNode
{
    symbol_t        _Symbol;
    size_t          _Frequency;
    _HuffmanNode*   _Left;
    _HuffmanNode*   _Right;

    _HuffmanNode() = delete;
    ~_HuffmanNode() = default;

    _HuffmanNode(symbol_t symbol, size_t frequency, _HuffmanNode* left = nullptr, _HuffmanNode* right = nullptr);

    _HuffmanNode(const _HuffmanNode& other) = delete;
    _HuffmanNode& operator=(const _HuffmanNode& other) = delete;
};  // END _HuffmanNode


struct _HuffmanNodePtrGreaterCompare
{
    bool operator()(const _HuffmanNode* const left, const _HuffmanNode* const right);
};  // END _HuffmanNodePtrGreaterCompare