#ifndef JNP1_6_COMPUTER_COMPONENTS_H
#define JNP1_6_COMPUTER_COMPONENTS_H

#include <memory>
#include <unordered_map>
#include <string_view>

class Memory {
public:
    using word_t = int64_t;
    using id_t = std::string_view;
private:
    class OutOfRangeMemoryAccessException : std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Address larger than size of memory cannot be accessed!";
        }
    };

    using mem_t = word_t[];
    size_t _size;
    std::unique_ptr<mem_t> mem;
    std::unordered_map<std::string_view , size_t> vars;
public:
    explicit Memory(size_t size) : _size(size), mem(std::make_unique<mem_t>(size)) {
        for (size_t i = 0; i < _size; ++i) {
            mem[i] = 0;
        }
    }

    [[nodiscard]] size_t size() const {
        return _size;
    }

    [[nodiscard]] word_t at(size_t i) const {
        if (i >= size()) {
            throw OutOfRangeMemoryAccessException();
        }
        return mem[i];
    }

    void set(size_t i, word_t new_val) {
        mem[i] = new_val;
    }

    [[nodiscard]] word_t get_variable_address(id_t var_name) const {
        return vars.at(var_name);
    }

    void add_variable(id_t var_name, word_t word) {
        size_t idx = vars.size();
        vars[var_name] = idx;
        set(idx, word);
    }
};

class ProcessorAbstract {
private:
    using flag_t = bool;
    flag_t ZF, SF;
protected:
    Memory &mem;
public:
    explicit ProcessorAbstract(Memory &memory)
            : ZF(false), SF(false), mem(memory) {}

    [[nodiscard]] flag_t getZF() const {
        return ZF;
    }

    [[nodiscard]] flag_t getSF() const {
        return SF;
    }

    void setZF(flag_t new_val) {
        ZF = new_val;
    }

    void setSF(flag_t new_val) {
        SF = new_val;
    }
};


#endif //JNP1_6_COMPUTER_COMPONENTS_H
