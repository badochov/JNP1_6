#ifndef JNP1_6_INSTRUCTION_H
#define JNP1_6_INSTRUCTION_H

#include "computer_components.h"

class Instruction {
public:
    virtual void execute(ProcessorAbstract &, Memory &) const = 0;

    virtual void declare(Memory &) const {};
};

#endif //JNP1_6_INSTRUCTION_H
