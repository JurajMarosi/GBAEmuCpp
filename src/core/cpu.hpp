#pragma once

#include "core/bus.hpp"
#include "core/decoder.hpp"

#include <cstdint>

class CPU {
  private:
    uint32_t regs[16];
    uint32_t cpsr;
    uint32_t currentIns;
    DecodedInstruction decodedIns;

    Decoder decoder;
    Bus &memBus;

    using ExecuteHandler = void (CPU::*)(const DecodedInstruction &);
    std::array<ExecuteHandler, 17> executeLut;

    void executeMultiply(const DecodedInstruction &decIns);
    void executeMultiplyLong(const DecodedInstruction &decIns);
    void executeBranchAndExchange(const DecodedInstruction &decIns);
    void executeSingleDataSwap(const DecodedInstruction &decIns);
    void executeHalfwordDataTransferReg(const DecodedInstruction &decIns);
    void executeHalfwordDataTransferImm(const DecodedInstruction &decIns);
    void executeSignedDataTransfer(const DecodedInstruction &decIns);
    void executeDataProcessing(const DecodedInstruction &decIns);
    void executeLoadStore(const DecodedInstruction &decIns);
    void executeUndefined(const DecodedInstruction &decIns);
    void executeBlockDataTransfer(const DecodedInstruction &decIns);
    void executeBranch(const DecodedInstruction &decIns);
    void executeCoprocessorDataTransfer(const DecodedInstruction &decIns);
    void executeCoprocessorDataOperation(const DecodedInstruction &decIns);
    void executeCoprocessorRegisterTransfer(const DecodedInstruction &decIns);
    void executePSRTransfer(const DecodedInstruction &decIns);
    void executeSoftwareInterrupt(const DecodedInstruction &decIns);

  public:
    enum Register { R0 = 0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, SP = 13, LR = 14, PC = 15 };

    CPU(Bus &bus);
    ~CPU();

    void fetch();

    void decode();

    void execute();

    void step();
};