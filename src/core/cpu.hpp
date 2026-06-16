#pragma once

#include "core/bus.hpp"

#include <cstdint>

class CPU {
  private:
    uint32_t regs[16];
    uint32_t cpsr;
    uint32_t currentIns;

    Bus &bus;

  public:
    enum Register {
        R0 = 0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        SP = 13,
        LR = 14,
        PC = 15
    };

    CPU(Bus &bus);
    ~CPU();

    void fetch();

    void decode();

    void execute();

    void step();
};