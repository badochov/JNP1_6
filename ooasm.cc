#include "instruction.h"
#include "ooasm.h"

class Data : public Instruction {
    ID id;
    const PValue *value;
public:
    Data(ID::id_t id, const PValue *value) : id(id), value(value) {}

    void execute(ProcessorAbstract &, Memory &memory) const override {
    }

    void declare(Memory &memory) const override {
        memory.add_variable(id.get(), value->get(memory));
    }
};

class Mov : public Instruction {
    const LValue *dst;
    const PValue *src;
public:
    Mov(const LValue *dst, const PValue *src) : dst(dst), src(src) {}

    void execute(ProcessorAbstract &, Memory &memory) const override {
        dst->set(memory, src->get(memory));
    }
};

class ArithmeticOperation : public Instruction {
    const LValue *arg1;
    const PValue *arg2;

    void set_value(word_t res, Memory &memory) const {
        arg1->set(memory, res);
    }

    static void set_flags(word_t res, ProcessorAbstract &processorAbstract) {
        processorAbstract.setSF(res < 0);
        processorAbstract.setZF(res == 0);
    }

    [[nodiscard]] virtual word_t function(word_t arg1, word_t arg2) const = 0;

protected:
    ArithmeticOperation(const LValue *arg1, const PValue *arg2) : arg1(arg1), arg2(arg2) {}

public:

    void execute(ProcessorAbstract &processorAbstract, Memory &memory) const override {
        word_t res = function(arg1->get(memory), arg2->get(memory));
        set_flags(res, processorAbstract);
        set_value(res, memory);
    }
};

class Add : public ArithmeticOperation {
    [[nodiscard]] word_t function(word_t arg1, word_t arg2) const override {
        return arg1 + arg2;
    }

public:
    Add(const LValue *arg1, const PValue *arg2) : ArithmeticOperation(arg1, arg2) {}
};

class Sub : public ArithmeticOperation {
    [[nodiscard]] word_t function(word_t arg1, word_t arg2) const override {
        return arg1 - arg2;
    }

public:
    Sub(const LValue *arg1, const PValue *arg2) : ArithmeticOperation(arg1, arg2) {}
};

class One : public Instruction {

    const LValue *lValue;
public:
    explicit One(const LValue *lValue) : lValue(lValue) {}

    void execute(ProcessorAbstract &processorAbstract, Memory &memory) const override {
        if (should_set(processorAbstract)) {
            lValue->set(memory, 1);
        }
    }

protected:

    [[nodiscard]]  virtual bool should_set(const ProcessorAbstract &) const {
        return true;
    }
};

class OneZ : public One {

public:
    explicit OneZ(const LValue *lValue) : One(lValue) {}

protected:
    [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
        return processorAbstract.getZF();
    }
};

class OneS : public One {

public:
    explicit OneS(const LValue *lValue) : One(lValue) {}

protected:
    [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
        return processorAbstract.getSF();
    }
};

Num *num(word_t word) {
    return new Num(word);
}

Mem *mem(const PValue *addr) {
    return new Mem(addr);
}

LEA *lea(ID::id_t id) {
    return new LEA(id);
}

Instruction *data(ID::id_t id, const PValue *value) {
    return new Data(id, value);
}

Instruction *mov(const LValue *dst, const PValue *src) {
    return new Mov(dst, src);
}

Instruction *add(const LValue *arg1, const PValue *arg2) {
    return new Add(arg1, arg2);
}

Instruction *sub(const LValue *arg1, const PValue *arg2) {
    return new Sub(arg1, arg2);
}

Instruction *inc(const LValue *arg) {
    return new Add(arg, num(1));
}

Instruction *dec(const LValue *arg) {
    return new Sub(arg, num(1));
}

Instruction *one(const LValue *arg) {
    return new One(arg);
}

Instruction *onez(const LValue *arg) {
    return new OneZ(arg);
}

Instruction *ones(const LValue *arg) {
    return new OneS(arg);
}
