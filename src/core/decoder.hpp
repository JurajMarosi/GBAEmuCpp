#pragma once
#include <array>
#include <cstdint>

enum class InstructionCond : uint8_t {
    EQ = 0,  // 0b0000 - Equal
    NE = 1,  // 0b0001 - Not Equal
    CS = 2,  // 0b0010 - Carry Set / Unsigned higher or same
    CC = 3,  // 0b0011 - Carry Clear / Unsigned lower
    MI = 4,  // 0b0100 - Minus / Negative
    PL = 5,  // 0b0101 - Plus / Positive or zero
    VS = 6,  // 0b0110 - Overflow
    VC = 7,  // 0b0111 - No Overflow
    HI = 8,  // 0b1000 - Unsigned higher
    LS = 9,  // 0b1001 - Unsigned lower or same
    GE = 10, // 0b1010 - Signed greater than or equal
    LT = 11, // 0b1011 - Signed less than
    GT = 12, // 0b1100 - Signed greater than
    LE = 13, // 0b1101 - Signed less than or equal
    AL = 14, // 0b1110 - Always
    NV = 15  // 0b1111 - Never
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
    PSRTransfer,
    SoftwareInterrupt
};

struct DecodedInstruction {
    InstructionType type; // bitNum: 27-25 or 7-4
    InstructionCond cond; // bitNum: 31-28 (all instructions)

    uint8_t rd; // bitNum: 15-12 (also Rd_LSW or CRd)
    uint8_t rn; // bitNum: 19-16 (also Rd_MSW or CRn)
    uint8_t rm; // bitNum: 3-0   (also CRm)
    uint8_t rs; // bitNum: 11-8  (also CP# or Rn in instruction MultiplyLong)
    uint8_t cp; // bitNum: 7-5

    uint8_t opcode; // bitNum: 24-21 (ADD, SUB... or CP opcode)

    bool S_L_bit;     // bitNum: 20 (CPSR or Load/Store)
    bool W_A_bit;     // bitNum: 21 (Writeback or Accumulate)
    bool B_U_N_S_bit; // bitNum: 22 (Byte/Halfword or Coprocessor)
    bool U_bit;       // bitNum: 23 (Up/Down offset)
    bool P_L_bit;     // bitNum: 24 (Pre/Post indexing)
    bool I_bit;       // bitNum: 25 (only for Load/store register/u byte)
    bool H_bit;       // bitNum: 5 (Halfword sign bit)

    union {
        uint32_t operand2;      // second operand in data ops
        uint32_t offset;        // for offsets and addr_mode
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
    static void decodeMultiply(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeMultiplyLong(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeBranchAndExchange(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeSingleDataSwap(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeHalfwordDataTransferReg(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeHalfwordDataTransferImm(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeSignedDataTransfer(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeDataProcessing(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeLoadStore(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeBlockDataTransfer(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeBranch(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeCoprocessorDataTransfer(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeCoprocessorDataOperation(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeCoprocessorRegisterTransfer(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodePSRTransfer(uint32_t rawIns, DecodedInstruction &decIns);
    static void decodeSoftwareInterrupt(uint32_t rawIns, DecodedInstruction &decIns);
};