#include "decoder.hpp"

static void initLUT() {}

static DecodedInstruction decode(uint32_t rawIns) {}

static void decodeMultiply() {}

static void decodeMultiplyLong() {}

static void decodeBranchAndExchange() {}

static void decodeSingleDataSwap() {}

static void decodeHalfwordDataTransferReg() {}

static void decodeHalfwordDataTransferImm() {}

static void decodeSignedDataTransfer() {}

static void decodeDataProcessing() {}

static void decodeLoadStore() {}

static void decodeUndefined() {}

static void decodeBlockDataTransfer() {}

static void decodeBranch() {}

static void decodeCoprocessorDataTransfer() {}

static void decodeCoprocessorDataOperation() {}

static void decodeCoprocessorRegisterTransfer() {}

static void decodeSoftwareInterrupt() {}

static InstructionType determineInsType(uint32_t rawIns) {
    uint32_t bits_27_26 = (rawIns >> 26) & 0b11;
    uint32_t bits_27_20 = (rawIns >> 20) & 0b11111111;
    uint32_t bit_25 = (rawIns >> 25) & 0b1;
    uint32_t bits_7_4 = (rawIns >> 4) & 0b1111;
    uint32_t bits_7_6 = (rawIns >> 6) & 0b11;
    uint32_t bit_24 = (rawIns >> 24) & 0b1;
    uint32_t bit_23 = (rawIns >> 23) & 0b1;
    uint32_t bit_22 = (rawIns >> 22) & 0b1;
    uint32_t bit_4 = (rawIns >> 4) & 0b1;

    if (bits_27_26 == 0b00) {
        if (bit_25 == 0b1) {
            return InstructionType::DataProcessing;
        }
        if (bits_7_4 == 0b1001) {
            if (bit_24 == 0b0 && bit_23 == 0b0) {
                return InstructionType::Multiply;
            }
            if (bit_24 == 0b0 && bit_23 == 0b1) {
                return InstructionType::MultiplyLong;
            }
            if (bit_24 == 0b1 && bit_23 == 0b0) {
                return InstructionType::SingleDataSwap;
            }
            return InstructionType::Undefined;
        } else if (bits_7_4 == 0b1011) {
            if (bit_22 == 0b0) {
                return InstructionType::HalfwordDataTransferReg;
            }
            if (bit_22 == 0b1) {
                return InstructionType::HalfwordDataTransferImm;
            }
        } else if (bits_7_6 == 0b11 && bit_4 == 0b1) {
            return InstructionType::SignedDataTransfer;
        } else if (bits_7_4 == 0b0001 && bits_27_20 == 0b00010010) {
            return InstructionType::BranchAndExchange;
        } else {
            return InstructionType::DataProcessing;
        }
    } else if (bits_27_26 == 0b01) {
        if (bit_25 == 0b1 && bit_4 == 0b1) {
            return InstructionType::Undefined;
        } else {
            return InstructionType::LoadStore;
        }
    } else if (bits_27_26 == 0b10) {
        if (bit_25 == 0b0) {
            return InstructionType::BlockDataTransfer;
        }
        if (bit_25 == 0b1) {
            return InstructionType::Branch;
        }
    } else if (bits_27_26 == 0b11) {
        if (bit_25 == 0b0) {
            return InstructionType::CoprocessorDataTransfer;
        }
        if (bit_25 == 0b1) {
            if (bit_24 == 0b0) {
                if (bit_4 == 0b0) {
                    return InstructionType::CoprocessorDataOperation;
                }
                if (bit_4 == 0b1) {
                    return InstructionType::CoprocessorRegisterTransfer;
                }
            } else {
                return InstructionType::SoftwareInterrupt;
            }
        }
    }
    return InstructionType::Undefined;
}