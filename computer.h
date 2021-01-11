#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "ooasm.h"
#include <ostream>
#include "computer_components.h"

class Processor : public ProcessorAbstract {
public:
    explicit Processor(Memory& mem) : ProcessorAbstract(mem) {}
    void execute(Instruction& ins) {
        ins.execute();
    }
};


class Computer {
private:
    Memory mem;
    Processor proc;
public:
    explicit Computer(size_t mem_size) : mem(mem_size), proc(mem) {

    }

    void boot(const Program& p) {
        for (const Instruction& ins : p) {
            proc.
        }
    }

    void memory_dump(const std::ostream& os) {
        for (size_t i = 0; i < mem.size(); ++i) {
            os << static_cast<long long>(mem.at(i)) << " ";
        }
    };
};

#endif //JNP1_6_COMPUTER_H
