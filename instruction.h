#ifndef JNP1_6_INSTRUCTION_H
#define JNP1_6_INSTRUCTION_H

#include "computer_components.h"

namespace ooasm {
    using computer::ProcessorAbstract;
    using computer::Memory;

    class Instruction {
    public:
        virtual ~Instruction() = default;

        virtual void execute(ProcessorAbstract &, Memory &) const = 0;

        virtual void declare(Memory &) const {};
    };
}

#endif //JNP1_6_INSTRUCTION_H
