#ifndef JNP1_6_COMPUTER_COMPONENTS_H
#define JNP1_6_COMPUTER_COMPONENTS_H

#include <memory>
#include <unordered_map>
#include <string>

namespace computer {
    class Memory {
    public:
        using word_t = int64_t;
        using address_t = uint64_t;
        using mem_size_t = uint64_t;
        using id_t = std::string;

        explicit Memory(mem_size_t size) : _size(size), mem(std::make_unique<mem_t>(size)) {
            for (mem_size_t i = 0; i < _size; ++i) {
                mem[i] = 0;
            }
        }

        [[nodiscard]] word_t at(address_t i) const {
            if (i >= size()) {
                throw OutOfRangeMemoryAccessException();
            }
            return mem[i];
        }

        void set(address_t i, word_t new_val) {
            if (i >= size()) {
                throw OutOfRangeMemoryAccessException();
            }
            mem[i] = new_val;
        }

        [[nodiscard]] address_t get_variable_address(const id_t &var_name) const {
            return vars.at(var_name);
        }

        void add_variable(const id_t &var_name, word_t word) {
            if (variables_count == size()) {
                throw TooManyVariablesException();
            }
            set(variables_count, word);
            if (vars.count(var_name) == 0) {
                vars[var_name] = variables_count;
            }
            variables_count++;
        }

        [[nodiscard]] mem_size_t size() const {
            return _size;
        }

        void wipe() {
            for (address_t i = 0; i < size(); i++) {
                mem[i] = 0;
            }

            vars.clear();
            variables_count = 0;
        }

    private:
        class OutOfRangeMemoryAccessException : public std::exception {
            [[nodiscard]] const char *what() const noexcept override {
                return "Address larger than size of memory cannot be accessed!";
            }
        };

        class TooManyVariablesException : public std::exception {
            [[nodiscard]] const char *what() const noexcept override {
                return "Too many variable declarations!";
            }
        };

        using mem_t = word_t[];
        mem_size_t _size;
        mem_size_t variables_count = 0;
        std::unique_ptr<mem_t> mem;
        std::unordered_map<id_t, mem_size_t> vars;

    };

    class ProcessorAbstract {
    public:
        using flag_t = bool;

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

    protected:
        flag_t ZF, SF;
        Memory &mem;

        explicit ProcessorAbstract(Memory &memory)
                : ZF(false), SF(false), mem(memory) {}

    };
}

#endif //JNP1_6_COMPUTER_COMPONENTS_H
