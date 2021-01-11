#ifndef JNP1_6_COMPUTER_COMPONENTS_H
#define JNP1_6_COMPUTER_COMPONENTS_H

#include <memory>

class Memory {
public:
    using word_t = int64_t;
private:
    using mem_t = word_t[];
    size_t _size;
    std::unique_ptr<mem_t> mem;
public:
    explicit Memory(size_t size)
    : _size(size), mem(std::make_unique<mem_t>(size)) {
        for (size_t i = 0; i < _size; ++i) {
            mem[i] = 0;
        }
    }
    size_t size() {
        return _size;
    }
    word_t at(size_t i) {
        return mem[i];
    }
    void set(size_t i, word_t new_val) {
        mem[i] = new_val;
    }
};

class ProcessorAbstract {
private:
    using flag_t = bool;
    flag_t ZF, SF;
    Memory& mem;
public:
    explicit ProcessorAbstract(Memory& memory)
            : ZF(false), SF(false), mem(memory) {}
    flag_t getZF() {
        return ZF;
    }
    flag_t getSF() {
        return SF;
    }
    void setZF(flag_t new_val) {
        ZF = new_val;
    }
    void setSF(flag_t new_val) {
        SF = new_val;
    }
};



#endif //JNP1_6_COMPUTER_COMPONENTS_H
