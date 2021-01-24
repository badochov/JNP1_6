#include "instruction.h"
#include "ooasm.h"

namespace ooasm {
    class Data : public Instruction {
    public:
        Data(ID::id_t _id, std::unique_ptr<Num> _value)
            : id(_id), value(std::move(_value)) {}

        void execute(ProcessorAbstract &, Memory &) const override {}

        void declare(Memory &memory) const override {
            memory.add_variable(id.get(), value->get(memory));
        }

    private:
        ID id;
        const std::unique_ptr<Num> value;
    };

    class Mov : public Instruction {
    public:
        Mov(std::unique_ptr<LValue> _dst, std::unique_ptr<RValue> _src)
                : dst(std::move(_dst)), src(std::move(_src)) {}

        void execute(ProcessorAbstract &, Memory &memory) const override {
            dst->set(memory, src->get(memory));
        }

    private:
        const std::unique_ptr<LValue> dst;
        const std::unique_ptr<RValue> src;
    };

    // Abstract class for handling any arithmetic operation performed in ooasm.
    // Deriving classes should overwrite function function to choose performed operation.
    class ArithmeticOperation : public Instruction {
    public:
        void execute(ProcessorAbstract &processorAbstract, Memory &memory) const override {
            word_t res = function(arg1->get(memory), arg2->get(memory));
            set_flags(res, processorAbstract);
            set_value(res, memory);
        }

        ~ArithmeticOperation() override = default;

    protected:
        ArithmeticOperation(std::unique_ptr<LValue> _arg1, std::unique_ptr<RValue> _arg2)
                : arg1(std::move(_arg1)), arg2(std::move(_arg2)) {}

    private:
        const std::unique_ptr<LValue> arg1;
        const std::unique_ptr<RValue> arg2;

        void set_value(word_t res, Memory &memory) const {
            arg1->set(memory, res);
        }

        static void set_flags(word_t res, ProcessorAbstract &processorAbstract) {
            processorAbstract.setSF(res < 0);
            processorAbstract.setZF(res == 0);
        }

        // Function to be applied on given values.
        [[nodiscard]] virtual word_t function(word_t a1, word_t a2) const = 0;
    };

    class Add : public ArithmeticOperation {
    public:
        Add(std::unique_ptr<LValue> _arg1, std::unique_ptr<RValue> _arg2)
                : ArithmeticOperation(std::move(_arg1), std::move(_arg2)) {}

    private:
        [[nodiscard]] word_t function(word_t a1, word_t a2) const override {
            return a1 + a2;
        }

    };

    class Sub : public ArithmeticOperation {
    public:
        Sub(std::unique_ptr<LValue> _arg1, std::unique_ptr<RValue> _arg2)
                : ArithmeticOperation(std::move(_arg1), std::move(_arg2)) {}

    private:
        [[nodiscard]] word_t function(word_t a1, word_t a2) const override {
            return a1 - a2;
        }
    };

    // Base class for setting one at given position.
    // Deriving classes should override should_set function to choose when 1 should be set.
    class One : public Instruction {
    public:
        explicit One(std::unique_ptr<LValue> _lValue) : lValue(std::move(_lValue)) {}

        void execute(ProcessorAbstract &processorAbstract, Memory &memory) const override {
            if (should_set(processorAbstract)) {
                lValue->set(memory, 1);
            }
        }

        ~One() override = default;

    protected:
        [[nodiscard]] virtual bool should_set(const ProcessorAbstract &) const {
            return true;
        }

    private:
        std::unique_ptr<LValue> lValue;
    };

    class OneZ : public One {
    public:
        explicit OneZ(std::unique_ptr<LValue> _lValue) : One(std::move(_lValue)) {}

    protected:
        [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
            return processorAbstract.getZF();
        }
    };

    class OneS : public One {
    public:
        explicit OneS(std::unique_ptr<LValue> _lValue) : One(std::move(_lValue)) {}

    protected:
        [[nodiscard]] bool should_set(const ProcessorAbstract &processorAbstract) const override {
            return processorAbstract.getSF();
        }
    };
}

using namespace ooasm;

std::unique_ptr<Num> num(word_t word) {
    return std::make_unique<Num>(word);
}

std::unique_ptr<Mem> mem(std::unique_ptr<RValue> addr) {
    return std::make_unique<Mem>(std::move(addr));
}

std::unique_ptr<LEA> lea(ID::id_t id) {
    return std::make_unique<LEA>(id);
}

std::shared_ptr<Instruction> data(ID::id_t id, std::unique_ptr<Num> value) {
    return std::make_shared<Data>(id, std::move(value));
}

std::shared_ptr<Instruction> mov(std::unique_ptr<LValue> dst, std::unique_ptr<RValue> src) {
    return std::make_shared<Mov>(std::move(dst), std::move(src));
}

std::shared_ptr<Instruction> add(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2) {
    return std::make_shared<Add>(std::move(arg1), std::move(arg2));
}

std::shared_ptr<Instruction> sub(std::unique_ptr<LValue> arg1, std::unique_ptr<RValue> arg2) {
    return std::make_shared<Sub>(std::move(arg1), std::move(arg2));
}

std::shared_ptr<Instruction> inc(std::unique_ptr<LValue> arg) {
    return std::make_shared<Add>(std::move(arg), num(1));
}

std::shared_ptr<Instruction> dec(std::unique_ptr<LValue> arg) {
    return std::make_shared<Sub>(std::move(arg), num(1));
}

std::shared_ptr<Instruction> one(std::unique_ptr<LValue> arg) {
    return std::make_shared<One>(std::move(arg));
}

std::shared_ptr<Instruction> onez(std::unique_ptr<LValue> arg) {
    return std::make_shared<OneZ>(std::move(arg));
}

std::shared_ptr<Instruction> ones(std::unique_ptr<LValue> arg) {
    return std::make_shared<OneS>(std::move(arg));
}