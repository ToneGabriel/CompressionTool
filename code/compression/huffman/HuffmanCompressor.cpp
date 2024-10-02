#include "HuffmanCompressor.h"



HuffmanCompressor::~HuffmanCompressor()
{
    _delete_tree();
}

void HuffmanCompressor::compress(const std::string& filename)
{
    _ASSERT(fs::exists(filename), "File not found!");

    _originalFilename   = filename;
    _compressedFilename = fs::path(filename).replace_extension(HUFFMAN_EXTENSION).string();

    std::ifstream ifile(_originalFilename, std::ios::binary);
    std::ofstream ofile(_compressedFilename, std::ios::binary);

    _compute_symbol_frequencies(ifile);
    _build_tree();
    _generate_huffman_codes();
    _write_frequency_map(ofile);
    _encode_and_write_file(ifile, ofile);
    _delete_tree();

    ifile.close();
    ofile.close();

    _originalSize   = _compute_file_size(_originalFilename);
    _compressedSize = _compute_file_size(_compressedFilename);
}

void HuffmanCompressor::decompress(const std::string& filename)
{
    _ASSERT(fs::exists(filename), "File not found!");

    _originalFilename   = fs::path(filename).replace_extension(".txtd").string();
    _compressedFilename = filename;

    std::ifstream ifile(_compressedFilename, std::ios::binary);
    std::ofstream ofile(_originalFilename, std::ios::binary);

    _read_frequency_map(ifile);
    _build_tree();
    _decode_and_write_file(ifile, ofile);
    _delete_tree();

    ifile.close();
    ofile.close();

    _originalSize   = _compute_file_size(_originalFilename);
    _compressedSize = _compute_file_size(_compressedFilename);
}

std::string HuffmanCompressor::get_original_file_name() const
{
    return _originalFilename;
}

std::string HuffmanCompressor::get_compressed_file_name() const
{
    return _compressedFilename;
}

size_t HuffmanCompressor::get_original_file_size() const   // in bytes
{
    return _originalSize;
}

size_t HuffmanCompressor::get_compressed_file_size() const // in bytes
{
    return _compressedSize;
}

double HuffmanCompressor::get_compression_ratio() const
{
    return 1.0 - static_cast<double>(_compressedSize) / static_cast<double>(_originalSize);
}

size_t HuffmanCompressor::_compute_file_size(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // open in binary mode and go to the end
    return static_cast<size_t>(file.tellg());
}

void HuffmanCompressor::_build_tree()
{
    _ASSERT(!_frequencyMap.empty(), "No frequency data!");

    // create leaf nodes for each symbol and push them to the priority queue
    std::priority_queue<_HuffmanNode*,
                        std::vector<_HuffmanNode*>,
                        _HuffmanNodePtrGreaterCompare> prioq;

    _HuffmanNode* leftNode     = nullptr;
    _HuffmanNode* rightNode    = nullptr;

    for (const auto& [symbol, frequency] : _frequencyMap)
        prioq.push(new _HuffmanNode(symbol, frequency));

    while (prioq.size() > 1)    // Last elem will be root
    {
        // remove two nodes of lowest frequency
        leftNode = prioq.top();
        prioq.pop();

        rightNode = prioq.top();
        prioq.pop();
        
        // create a new internal subroot with these two nodes as children
        prioq.push(new _HuffmanNode(   static_cast<symbol_t>(0),
                                        leftNode->_Frequency + rightNode->_Frequency,
                                        leftNode,
                                        rightNode));
    }

    // last elem is root
    _root = prioq.top();
    prioq.pop();
}

void HuffmanCompressor::_delete_tree_impl(_HuffmanNode* subroot)
{
    if (subroot == nullptr)
        return;

    _delete_tree_impl(subroot->_Left);
    _delete_tree_impl(subroot->_Right);

    delete subroot;
}

void HuffmanCompressor::_delete_tree()
{
    _delete_tree_impl(_root);
}

void HuffmanCompressor::_print_tree_impl(const size_t ident, const _HuffmanNode* const subroot, const std::string& property) const
{
    if (subroot != nullptr)
        std::cout   << std::string("").append(ident, '\t')
                    << subroot->_Symbol
                    << " [" << subroot->_Frequency << " "<< property << "]\n";

    if (subroot->_Left != nullptr)
        _print_tree_impl(ident + 1, subroot->_Left, "LEFT");

    if (subroot->_Right != nullptr)
        _print_tree_impl(ident + 1, subroot->_Right, "RIGHT");
}

void HuffmanCompressor::_print_tree() const
{
    _print_tree_impl(0, _root, "ROOT");
}

void HuffmanCompressor::_compute_symbol_frequencies(std::ifstream& ifile)
{
    symbol_t symbol;

    while (ifile.get(symbol))
        ++_frequencyMap[symbol];

    ifile.clear();              // clear error flags
    ifile.seekg(std::ios::beg); // reset file to beginning
}

void HuffmanCompressor::_generate_huffman_codes_impl(  const _HuffmanNode* const subroot,
                                    const std::string& code,
                                    std::unordered_map<symbol_t, std::string>& codes)
{
    if (subroot == nullptr)
        return;

    // if it's a leaf node, store the code
    if (subroot->_Left == nullptr && subroot->_Right == nullptr)
    {
        codes[subroot->_Symbol] = code;
        return;
    }

    // traverse left subtree, append '0'
    _generate_huffman_codes_impl(subroot->_Left, code + ZERO_CHR, codes);

    // traverse right subtree, append '1'
    _generate_huffman_codes_impl(subroot->_Right, code + ONE_CHR, codes);
}

void HuffmanCompressor::_generate_huffman_codes()
{
    _generate_huffman_codes_impl(_root, "", _codeMap);
}

void HuffmanCompressor::_write_frequency_map(std::ofstream& ofile)
{
    _ASSERT(ofile.is_open(), "Output file is not open!");

    size_t mapSize = _frequencyMap.size();
    ofile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

    for (const auto& [symbol, count] : _frequencyMap)
    {
        ofile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
        ofile.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }

    // DON'T close file
}

void HuffmanCompressor::_read_frequency_map(std::ifstream& ifile)
{
    _ASSERT(ifile.is_open(), "Input file is not open!");

    size_t      mapSize;
    symbol_t    symbol;
    size_t      count;

    ifile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    for (size_t i = 0; i < mapSize; ++i)
    {

        ifile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
        ifile.read(reinterpret_cast<char*>(&count), sizeof(count));

        _frequencyMap[symbol] = count;
    }
}

void HuffmanCompressor::_encode_and_write_file(std::ifstream& ifile, std::ofstream& ofile)
{
    _ASSERT(ifile.is_open(), "Input file is not open!");
    _ASSERT(ofile.is_open(), "Output file is not open!");

    std::string buffer;
    symbol_t    symbol;

    while (ifile.get(symbol))
    {
        buffer.append(_codeMap.at(symbol));

        while (buffer.size() >= SYMBOL_BIT)
        {
            // take necessary bits from buffer and transform them into bitset
            std::bitset<SYMBOL_BIT> bits(buffer, 0, SYMBOL_BIT);

            // remaining bits will be used in next iterations
            buffer.erase(0, SYMBOL_BIT);

            // transform bits from bitset into a symbol and write it
            ofile.put(static_cast<symbol_t>(bits.to_ulong()));
        }
    }

    // pad leftover bits in the buffer with 0s and write them as the last byte
    if (!buffer.empty())
    {
        // Pad the remaining bits to form a full byte
        buffer.append(SYMBOL_BIT - buffer.size(), ZERO_CHR);
        std::bitset<SYMBOL_BIT> bits(buffer);

        ofile.put(static_cast<symbol_t>(bits.to_ulong()));
    }

    // DON'T close files
}

void HuffmanCompressor::_decode_and_write_file(std::ifstream& ifile, std::ofstream& ofile)
{
    _ASSERT(ifile.is_open(), "Input file is not open!");
    _ASSERT(ofile.is_open(), "Output file is not open!");

    _HuffmanNode*  currentNode = _root;
    std::string     buffer;
    symbol_t        symbol;

    while (ifile.get(symbol))
    {
        std::bitset<SYMBOL_BIT> bits(symbol);
        buffer = std::move(bits.to_string());

        for (symbol_t digit : buffer)
        {
            if (digit == ZERO_CHR)
                currentNode = currentNode->_Left;
            else
                currentNode = currentNode->_Right;

            if (currentNode->_Left == nullptr && currentNode->_Right == nullptr)
            {
                ofile.put(currentNode->_Symbol);    // write the character to the output file
                currentNode = _root;                // reset to root for next character
            }
            else
            {
                // do nothing - currentNode remains the same
            }
        }
    }
}