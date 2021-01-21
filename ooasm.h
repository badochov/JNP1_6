#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <vector>
#include <cstring>
#include "instruction.h"
#include <iterator>

namespace {
    using word_t = Memory::word_t;
    using address_t = Memory::address_t;
}

class Program {
    using ins_t = const std::vector<std::shared_ptr<Instruction>>;
    ins_t ins;

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
};

class ID {
    constexpr static size_t MAX_LEN = 10;
    constexpr static size_t MIN_LEN = 1;
    std::unique_ptr<Memory::id_t> id;

public:
    using id_t = const char *;

    explicit ID(id_t _id) {
        if(_id == nullptr) {
            throw InvalidId();
        }
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
    class InvalidId : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Invalid ID! Id should be between 1 and 10 characters";
        }
    };
};

class Value {
public:
    virtual ~Value() = default;
};

class RValue : public Value {
public:
    [[nodiscard]] virtual word_t get(const Memory &) const = 0;

    [[nodiscard]] virtual address_t get_address(const Memory &memory) const {
        return get(memory);
    }

    ~RValue() override = default;
};

class LValue : public RValue {
public:
    virtual void set(Memory &, word_t) const = 0;

    ~LValue() override = default;
};

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

class Num : public RValue {
    word_t num;

public:
    explicit Num(word_t _num) : num(_num) {}

    [[nodiscard]] word_t get(const Memory &) const override {
        return num;
    }
};

class LEA : public RValue {
    ID id;

public:
    explicit LEA(ID::id_t _id) : id(_id) {}

    [[nodiscard]] address_t get_address(const Memory &memory) const override {
        return memory.get_variable_address(id.get());
    }

    [[nodiscard]] word_t get(const Memory &memory) const override {
        return get_address(memory);
    }
};


std::unique_ptr<Num> num(word_t word);

std::unique_ptr<Mem> mem(std::unique_ptr<RValue> addr);

std::unique_ptr<LEA> lea(ID::id_t id);

std::shared_ptr<Instruction> data(ID::id_t id, std::unique_ptr<Num> value);

std::shared_ptr<Instruction> mov(std::unique_ptr<LValue> dst, std::unique_ptr<RValue> src);

std::shared_ptr<Instruction> add(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2);

std::shared_ptr<Instruction> sub(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2);

std::shared_ptr<Instruction> inc(std::unique_ptr<LValue> arg);

std::shared_ptr<Instruction> dec(std::unique_ptr<LValue> arg);

std::shared_ptr<Instruction> one(std::unique_ptr<LValue> arg);

std::shared_ptr<Instruction> onez(std::unique_ptr<LValue> arg);

std::shared_ptr<Instruction> ones(std::unique_ptr<LValue> arg);


using program = Program;

#endif //JNP1_6_OOASM_H
