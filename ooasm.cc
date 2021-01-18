#include "instruction.h"
#include "ooasm.h"

class Data : public Instruction {
    ID id;
<<<<<<< HEAD
    const std::unique_ptr<Num> value;
public:
    Data(ID::id_t id, std::unique_ptr<Num> value) : id(id), value(std::move(value)) {}
=======
    const std::unique_ptr<RValue> value;
public:
    Data(ID::id_t id, std::unique_ptr<RValue> value) : id(id), value(std::move(value)) {}
>>>>>>> c707a5b4d15c8f8bfee6fe4bede25b9782c760a2

    void execute(ProcessorAbstract &, Memory &memory) const override {}

    void declare(Memory &memory) const override {
        memory.add_variable(id.get(), value->get(memory));
    }
};

class Mov : public Instruction {
    const std::unique_ptr<LValue> dst;
    const std::unique_ptr<RValue> src;
public:
    Mov(std::unique_ptr<LValue> dst, std::unique_ptr<RValue> src)
      : dst(std::move(dst)), src(std::move(src)) {}

    void execute(ProcessorAbstract &, Memory &memory) const override {
        dst->set(memory, src->get(memory));
    }
};

class ArithmeticOperation : public Instruction {
    const std::unique_ptr<LValue> arg1;
    const std::unique_ptr<RValue> arg2;

    void set_value(word_t res, Memory &memory) const {
        arg1->set(memory, res);
    }

    static void set_flags(word_t res, ProcessorAbstract &processorAbstract) {
        processorAbstract.setSF(res < 0);
        processorAbstract.setZF(res == 0);
    }

    [[nodiscard]] virtual word_t function(word_t arg1, word_t arg2) const = 0;

protected:
    ArithmeticOperation(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2)
      : arg1(std::move(arg1)), arg2(std::move(arg2)) {}

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
    Add(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2)
      : ArithmeticOperation(std::move(arg1), std::move(arg2)) {}
};

class Sub : public ArithmeticOperation {
    [[nodiscard]] word_t function(word_t arg1, word_t arg2) const override {
        return arg1 - arg2;
    }

public:
    Sub(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2)
      : ArithmeticOperation(std::move(arg1), std::move(arg2)) {}
};

class One : public Instruction {
  private:
    std::unique_ptr<LValue> lValue;
  public:
    explicit One(std::unique_ptr<LValue> lValue) : lValue(std::move(lValue)) {}

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
    explicit OneZ(std::unique_ptr<LValue> lValue) : One(std::move(lValue)) {}

protected:
    [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
        return processorAbstract.getZF();
    }
};

class OneS : public One {

public:
    explicit OneS(std::unique_ptr<LValue> lValue) : One(std::move(lValue)) {}

protected:
    [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
        return processorAbstract.getSF();
    }
};

std::unique_ptr<Num> num(word_t word) {
    return std::make_unique<Num>(word);
}

std::unique_ptr<Mem> mem(std::unique_ptr<RValue> addr) {
    return std::make_unique<Mem>(std::move(addr));
}

std::unique_ptr<LEA> lea(ID::id_t id) {
    return std::make_unique<LEA>(id);
}

<<<<<<< HEAD
std::unique_ptr<Instruction> data(ID::id_t id, std::unique_ptr<Num> value) {
=======
std::unique_ptr<Instruction> data(ID::id_t id, std::unique_ptr<RValue> value) {
>>>>>>> c707a5b4d15c8f8bfee6fe4bede25b9782c760a2
    return std::make_unique<Data>(id, std::move(value));
}

std::unique_ptr<Instruction> mov(std::unique_ptr<LValue> dst, std::unique_ptr<RValue> src) {
    return std::make_unique<Mov>(std::move(dst), std::move(src));
}

std::unique_ptr<Instruction> add(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2) {
    return std::make_unique<Add>(std::move(arg1), std::move(arg2));
}

std::unique_ptr<Instruction> sub(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2) {
    return std::make_unique<Sub>(std::move(arg1), std::move(arg2));
}

std::unique_ptr<Instruction> inc(std::unique_ptr<LValue> arg) {
    return std::make_unique<Add>(std::move(arg), num(1));
}

std::unique_ptr<Instruction> dec(std::unique_ptr<LValue> arg) {
    return std::make_unique<Sub>(std::move(arg), num(1));
}

std::unique_ptr<Instruction> one(std::unique_ptr<LValue> arg) {
    return std::make_unique<One>(std::move(arg));
}

std::unique_ptr<Instruction> onez(std::unique_ptr<LValue> arg) {
    return std::make_unique<OneZ>(std::move(arg));
}

std::unique_ptr<Instruction> ones(std::unique_ptr<LValue> arg) {
    return std::make_unique<OneS>(std::move(arg));
}
