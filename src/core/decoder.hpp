#pragma once
#include <array>
#include <cstdint>

enum class InstructionCond : uint8_t {
    EQ = 0,
    NE,
    CS,
    CC,
    MI,
    PL,
    VS,
    VC,
    HI,
    LS,
    GE,
    LT,
    GT,
    LE,
    AL,
    NV
};

enum class InstructionType {
    Multiply,
    MultiplyLong,
    BranchAndExchange,
    SingleDataSwap,
    HalfwordDataTransferReg,
    HalfwordDataTransferImm,
    SignedDataTransfer,
    DataProcessing,
    LoadStore,
    Undefined,
    BlockDataTransfer,
    Branch,
    CoprocessorDataTransfer,
    CoprocessorDataOperation,
    CoprocessorRegisterTransfer,
    SoftwareInterrupt
};

struct DecodedInstruction {
    InstructionCond cond; // bitNum: 31-28 (all instructions)
    InstructionType type; // bitNum: 27-25 or 7-4

    uint8_t rd; // bitNum: 15-12 (also Rd_LSW or CRd)
    uint8_t rn; // bitNum: 19-16 (also Rd_MSW or CRn)
    uint8_t rm; // bitNum: 3-0   (also CRm)
    uint8_t rs; // bitNum: 11-8

    uint8_t opcode; // bitNum: 24-21 (ADD, SUB... or CP opcode)

    bool S_L_bit;   // bitNum: 20 (CPSR or Load/Store)
    bool W_A_bit;   // bitNum: 21 (Writeback or Accumulate)
    bool B_U_N_bit; // bitNum: 22 (Byte/Halfword or Coprocessor)
    bool U_bit;     // bitNum: 23 (Up/Down offset)
    bool P_bit;     // bitNum: 24 (Pre/Post indexing)
    bool H_bit;     // bitNum: 5 (Halfword sign bit)

    union {
        uint32_t operand2;      // second operand in data ops
        uint32_t offset;        // for ins jumps and Load/Store offsets
        uint16_t register_list; // for Block data transfer (bitNum: 15-0)
    };
};

class Decoder {
  public:
    static void initLUT();
    static DecodedInstruction decode(uint32_t rawIns);

  private:
    static std::array<InstructionType, 4096> lut;
    static InstructionType determineInsType(uint32_t rawIns);
    static void decodeMultiply();
    static void decodeMultiplyLong();
    static void decodeBranchAndExchange();
    static void decodeSingleDataSwap();
    static void decodeHalfwordDataTransferReg();
    static void decodeHalfwordDataTransferImm();
    static void decodeSignedDataTransfer();
    static void decodeDataProcessing();
    static void decodeLoadStore();
    static void decodeUndefined();
    static void decodeBlockDataTransfer();
    static void decodeBranch();
    static void decodeCoprocessorDataTransfer();
    static void decodeCoprocessorDataOperation();
    static void decodeCoprocessorRegisterTransfer();
    static void decodeSoftwareInterrupt();
};