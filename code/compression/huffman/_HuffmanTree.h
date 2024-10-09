#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <queue>

#include "_HuffmanNode.h"


class _HuffmanTreeTraversor
{
private:

    _HuffmanNode* _current = nullptr;

public:

    _HuffmanTreeTraversor()     = default;
    ~_HuffmanTreeTraversor()    = default;

    _HuffmanTreeTraversor(_HuffmanNode* node);

public:

    const symbol_t& get_symbol() const;

    const size_t& get_frequency() const;

    _HuffmanTreeTraversor& operator+=(symbol_t digit);

    bool is_leaf() const;
};  // END _HuffmanTreeTraversor


class _HuffmanTree
{
private:

    _HuffmanNode* _root = nullptr;

public:

    _HuffmanTree() = default;

    ~_HuffmanTree();

public:

    _HuffmanTreeTraversor traversor_begin() const;

    void build(const std::map<symbol_t, size_t>& frequencyMap);

    std::unordered_map<symbol_t, std::string> generate_huffman_codes() const;

    void clear();

    void print() const;

    bool empty() const;

private:

    void _check_tree() const;

    void _clear_tree_impl(_HuffmanNode* subroot);

    void _print_tree_impl(  const size_t ident,
                            const _HuffmanNode* const subroot,
                            const std::string& property) const;

    void _generate_huffman_codes_impl(  const _HuffmanNode* const subroot,
                                        const std::string& code,
                                        std::unordered_map<symbol_t, std::string>& codes) const;
};  // END _HuffmanTree