#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "ooasm.h"
#include <ostream>

class Computer {
public:
    Computer(size_t s){
        mem = vector<mem_t>(s, 0);
    }

    void boot(const Program &p) {

    }

    void memory_dump(const std::ostream &os) {
        for(mem_t el : mem){
            os<<el<<" ";
        }
    };
private:
    using mem_t = int_fast64_t;
    vector <mem_t> mem;
};

#endif //JNP1_6_COMPUTER_H
