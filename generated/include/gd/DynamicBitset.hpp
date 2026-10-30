#pragma once

class DynamicBitset {
public:
    void resize(size_t size);

    std::vector<unsigned int> m_bits;

};
