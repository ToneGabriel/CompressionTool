#pragma once

#include <string>
#include <map>
#include <queue>
#include <unordered_map>

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
    bool is_leaf() const;

    _HuffmanTreeTraversor& operator+=(const symbol_t& digit);
};  // END _HuffmanTreeTraversor


class _HuffmanTree
{
private:

    _HuffmanNode* _root = nullptr;

public:

    _HuffmanTree() = default;
    ~_HuffmanTree();

    _HuffmanTree(const _HuffmanTree& other);
    _HuffmanTree(_HuffmanTree&& other) noexcept;

    _HuffmanTree& operator=(const _HuffmanTree& other);
    _HuffmanTree& operator=(_HuffmanTree&& other) noexcept;

public:

    _HuffmanTreeTraversor traversor_begin() const;

    void build(const std::map<symbol_t, size_t>& frequencyMap);
    std::unordered_map<symbol_t, std::string> generate_huffman_codes() const;

    void clear();
    void print() const;
    bool empty() const;

private:

    void _copy_tree(const _HuffmanTree& other);
    void _move_tree(_HuffmanTree&& other) noexcept;

    void _check_tree() const;
    void _clear_tree_impl(_HuffmanNode* subroot);
    void _print_tree_impl(  const size_t ident,
                            const _HuffmanNode* const subroot,
                            const std::string& property) const;
    void _generate_huffman_codes_impl(  const _HuffmanNode* const subroot,
                                        const std::string& code,
                                        std::unordered_map<symbol_t, std::string>& codes) const;
};  // END _HuffmanTree