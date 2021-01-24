#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "ooasm.h"
#include <ostream>
#include "computer_components.h"

// Implementation detail namespace concerning computer abstraction parts.
namespace computer {
    using ooasm::Instruction;

    // Derived class for processor with operations on ooasm instructions.
    class Processor : public ProcessorAbstract {
    public:
        explicit Processor(Memory &_mem) : ProcessorAbstract(_mem) {}

        void execute(const Instruction &ins) {
            ins.execute(*this, mem);
        }

        void declare(const Instruction &ins) {
            ins.declare(mem);
        }
    };

    // Class for abstract computer being environment of ooasm execution.
    class Computer {
    public:
        explicit Computer(size_t mem_size) : mem(mem_size), proc(mem) {}

        void boot(const ooasm::Program &p) {
            mem.wipe();

            for (const std::shared_ptr<Instruction> &ins : p) {
                proc.declare(*ins);
            }
            for (const std::shared_ptr<Instruction> &ins : p) {
                proc.execute(*ins);
            }
        }

        void memory_dump(std::ostream &os) const {
            for (size_t i = 0; i < mem.size(); ++i) {
                os << static_cast<long long>(mem.at(i)) << " ";
            }
        };
    private:
        Memory mem;
        Processor proc;
    };
}

using computer::Computer;

#endif //JNP1_6_COMPUTER_H
