#include "_HuffmanNode.h"


_HuffmanNode::_HuffmanNode(symbol_t symbol, size_t frequency, _HuffmanNode* left, _HuffmanNode* right)
    :   _Symbol(symbol),
        _Frequency(frequency),
        _Left(left),
        _Right(right)
{
    /*Empty*/
}


bool _HuffmanNodePtrGreaterCompare::operator()(const _HuffmanNode* const left, const _HuffmanNode* const right)
{
    if (left->_Frequency == right->_Frequency)
        return left->_Symbol > right->_Symbol;  // Tie-breaking: compare by symbol

    return left->_Frequency > right->_Frequency;
}