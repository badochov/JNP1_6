#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <vector>
#include <cstring>
#include "instruction.h"

class Program {
private:
    using ins_t = const std::vector<Instruction *>;
public:
    using iterator = ins_t::const_iterator;

    Program(const std::initializer_list<Instruction *> &instructions) : ins(instructions) {}

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

class PValue : public Value {
};

class Mem : public PValue, public LValue {
public:
    explicit Mem(const PValue *_addr) : addr(_addr) {}

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

    const PValue *addr;
};

class Num : public PValue {
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

class LEA : public PValue {
public:
    explicit LEA(ID::id_t _id) : id(_id) {}

    [[nodiscard]] word_t get(const Memory &memory) const override {
        return memory.get_variable_address(id.get());
    }

private:
    ID id;
};

Num *num(word_t word);

Mem *mem(const PValue *addr);

LEA *lea(ID::id_t id);

Instruction *data(ID::id_t id, const PValue *value);

Instruction *mov(const LValue *dst, const PValue *src);

Instruction *add(const LValue *arg1, const PValue *arg2);

Instruction *sub(const LValue *arg1, const PValue *arg2);

Instruction *inc(const LValue *arg);

Instruction *dec(const LValue *arg);

Instruction *one(const LValue *arg);

Instruction *onez(const LValue *arg);

Instruction *ones(const LValue *arg);

inline Program program(const std::initializer_list<Instruction *> &instructions) {
    return Program(instructions);
}

#endif //JNP1_6_OOASM_H
