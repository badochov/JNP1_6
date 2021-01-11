#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include "computer_components.h"
#include <vector>

class Instruction;

class Program {
private:
    using ins_t = std::vector<Instruction>;
public:
    using iterator = ins_t::const_iterator;
    Program(const std::initializer_list<Instruction> &instructions) : ins(instructions) {}
    iterator begin() const {
        return ins.begin();
    }
    iterator end() const {
        return ins.end();
    }
private:
    ins_t ins;
};

#endif //JNP1_6_OOASM_H
