#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <vector>
#include <cstring>
#include "instruction.h"
#include <iterator>

class Program {
private:
    using ins_t = const std::vector<std::shared_ptr<Instruction>>;
public:
    using iterator = ins_t::const_iterator;

    Program(const std::initializer_list<std::shared_ptr<Instruction>>& instructions)
            : ins(instructions) {}
//            : ins(std::make_move_iterator(instructions.begin()),
//                  std::make_move_iterator(instructions.end())) {}

    [[nodiscard]] iterator begin() const {
        return ins.begin();
    }

    [[nodiscard]] iterator end() const {
        return ins.end();
    }

private:
    ins_t ins;
};

using word_t = Memory::word_t;

class Value {

protected:
    virtual ~Value() = default;

public:
    [[nodiscard]] virtual word_t get(const Memory &) const = 0;
};

class LValue : public Value {
public:
    virtual void set(Memory &, word_t) const = 0;
};

class RValue : public Value {
};

class Mem : public RValue, public LValue {
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
        return addr->get(memory);
    }

    std::unique_ptr<RValue> addr;
};

class Num : public RValue {
    word_t num;
public:
    explicit Num(word_t _num) : num(_num) {}

    [[nodiscard]] word_t get(const Memory &) const override {
        return num;
    }
};


class ID {
    constexpr static size_t MAX_LEN = 10;
    constexpr static size_t MIN_LEN = 1;

    class InvalidId : std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Invalid ID! Id should be between 1 and 10 characters";
        }
    };

public:
    using id_t = const char *;

    explicit ID(id_t _id) {
        size_t len = strlen(_id);
        if (len < MIN_LEN || len > MAX_LEN) {
            throw InvalidId();
        }
        id = std::make_unique<Memory::id_t>(_id);
    }

    [[nodiscard]] Memory::id_t get() const {
        return *id;
    }

private:
    std::unique_ptr<Memory::id_t> id;
};

class LEA : public RValue {
public:
    explicit LEA(ID::id_t _id) : id(_id) {}

    [[nodiscard]] word_t get(const Memory &memory) const override {
        return memory.get_variable_address(id.get());
    }

private:
    ID id;
};

std::unique_ptr<Num> num(word_t word);

std::unique_ptr<Mem> mem(std::unique_ptr<RValue> addr);

std::unique_ptr<LEA> lea(ID::id_t id);

std::unique_ptr<Instruction> data(ID::id_t id, std::unique_ptr<Num> value);

std::unique_ptr<Instruction> mov(std::unique_ptr<LValue> dst, std::unique_ptr<RValue> src);

std::unique_ptr<Instruction> add(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2);

std::unique_ptr<Instruction> sub(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2);

std::unique_ptr<Instruction> inc(std::unique_ptr<LValue> arg);

std::unique_ptr<Instruction> dec(std::unique_ptr<LValue> arg);

std::unique_ptr<Instruction> one(std::unique_ptr<LValue> arg);

std::unique_ptr<Instruction> onez(std::unique_ptr<LValue> arg);

std::unique_ptr<Instruction> ones(std::unique_ptr<LValue> arg);


inline Program program(std::initializer_list<std::shared_ptr<Instruction>> instructions) {
    return Program(instructions);
}

#endif //JNP1_6_OOASM_H
