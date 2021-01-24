#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <vector>
#include <cstring>
#include "instruction.h"
#include <iterator>

// Implementation detail namespace concerning ooasm language.
namespace ooasm {
    using word_t = Memory::word_t;
    using address_t = Memory::address_t;

    // Program class used to create a program consisting of ooasm instructions.
    class Program {
        using ins_t = const std::vector<std::shared_ptr<Instruction>>;
    public:
        using iterator = ins_t::const_iterator;

        Program(std::initializer_list<std::shared_ptr<Instruction>> &&instructions)
                : ins(instructions) {}

        [[nodiscard]] iterator begin() const {
            return ins.begin();
        }

        [[nodiscard]] iterator end() const {
            return ins.end();
        }

    private:
        ins_t ins;
    };

    // Class for identifiers.
    class ID {
    public:
        using id_t = const char *;

        explicit ID(id_t _id) {
            if (_id == nullptr) {
                throw InvalidId();
            }
            size_t len = strlen(_id);
            if (len < MIN_LEN || len > MAX_LEN) {
                throw InvalidId();
            }

            id = Memory::id_t(_id);
        }

        [[nodiscard]] Memory::id_t get() const {
            return id;
        }

    private:
        // ID size constraints.
        constexpr static size_t MAX_LEN = 10;
        constexpr static size_t MIN_LEN = 1;

        Memory::id_t id;

        class InvalidId : public std::exception {
            [[nodiscard]] const char *what() const noexcept override {
                return "Invalid ID! ID length should be between 1 and 10 characters";
            }
        };
    };

    class RValue {
    public:
        // Pure virtual method to get value out of RValue.
        [[nodiscard]] virtual word_t get(const Memory &) const = 0;

        // Virtual method to get address value out of RValue.
        [[nodiscard]] virtual address_t get_address(const Memory &memory) const {
            return get(memory);
        }

        virtual ~RValue() = default;
    };

    class LValue : public RValue {
    public:
        // Pure virtual method to set address in memory to which LValue points to given word.
        virtual void set(Memory &, word_t) const = 0;

        virtual ~LValue() = default;
    };

    // Underlying class for mem language element.
    class Mem : public LValue {
    public:
        explicit Mem(std::unique_ptr<RValue> _addr) : addr(std::move(_addr)) {}

        [[nodiscard]] word_t get(const Memory &memory) const override {
            return memory.at(get_addr(memory));
        }

        void set(Memory &memory, word_t word) const override {
            memory.set(get_addr(memory), word);
        }

    private:
        [[nodiscard]] word_t get_addr(const Memory &memory) const {
            return addr->get_address(memory);
        }

        std::unique_ptr<RValue> addr;
    };

    // Underlying class for num language element.
    class Num : public RValue {
    public:
        explicit Num(word_t _num) : num(_num) {}

        [[nodiscard]] word_t get(const Memory &) const override {
            return num;
        }

    private:
        word_t num;
    };


    // Underlying class for lea language element.
    class LEA : public RValue {
    public:
        explicit LEA(ID::id_t _id) : id(_id) {}

        [[nodiscard]] address_t get_address(const Memory &memory) const override {
            return memory.get_variable_address(id.get());
        }

        [[nodiscard]] word_t get(const Memory &memory) const override {
            return get_address(memory);
        }

    private:
        ID id;
    };
}

// Language element which creates numeric literal.
std::unique_ptr<ooasm::Num> num(ooasm::word_t word);

// Language element which enables to access memory at given address.
std::unique_ptr<ooasm::Mem> mem(std::unique_ptr<ooasm::RValue> addr);

// Language element which enables to get address in memory of variable with given id.
std::unique_ptr<ooasm::LEA> lea(ooasm::ID::id_t id);

// Language instruction for declaring variable with given ID and setting it to given value.
std::shared_ptr<ooasm::Instruction> data(ooasm::ID::id_t id, std::unique_ptr<ooasm::Num> value);

// Language instruction for coping value from src to dst.
std::shared_ptr<ooasm::Instruction> mov(std::unique_ptr<ooasm::LValue> dst,
                                        std::unique_ptr<ooasm::RValue> src);

// Language instruction for adding arg2 to arg1 and storing it in arg1. Sets processor flags.
std::shared_ptr<ooasm::Instruction> add(std::unique_ptr<ooasm::LValue> arg1,
                                        std::unique_ptr<ooasm::RValue> arg2);

// Language instruction for subtracting arg2 to arg1 and storing it in arg1. Sets processor flags.
std::shared_ptr<ooasm::Instruction> sub(std::unique_ptr<ooasm::LValue> arg1,
                                        std::unique_ptr<ooasm::RValue> arg2);

// Language instruction for incrementing value. Sets processor flags.
std::shared_ptr<ooasm::Instruction> inc(std::unique_ptr<ooasm::LValue> arg);

// Language instruction for decrementing value. Sets processor flags.
std::shared_ptr<ooasm::Instruction> dec(std::unique_ptr<ooasm::LValue> arg);

// Language instruction for setting given variable to 1.
std::shared_ptr<ooasm::Instruction> one(std::unique_ptr<ooasm::LValue> arg);

// Language instruction for setting given variable to 1 if ZF is set.
std::shared_ptr<ooasm::Instruction> onez(std::unique_ptr<ooasm::LValue> arg);

// Language instruction for setting given variable to 1 if SF is set.
std::shared_ptr<ooasm::Instruction> ones(std::unique_ptr<ooasm::LValue> arg);

using program = ooasm::Program;

#endif //JNP1_6_OOASM_H
