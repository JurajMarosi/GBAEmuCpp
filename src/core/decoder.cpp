#include "decoder.hpp"

using namespace std;

array<InstructionType, 4096> Decoder::lut;

void Decoder::initLUT() {
    for (uint32_t i = 0; i < 4096; i++) {
        uint32_t fake_instruction = ((i & 0xFF0) << 16) | ((i & 0x00F) << 4);
        lut[i] = determineInsType(fake_instruction);
    }
}

DecodedInstruction Decoder::decode(uint32_t rawIns) {
    DecodedInstruction decodedIns;

    decodedIns.cond = static_cast<InstructionCond>(rawIns >> 28);

    uint32_t lutIndex = ((rawIns >> 16) & 0xFF0) | ((rawIns >> 4) & 0x00F);
    decodedIns.type = lut[lutIndex];

    switch (decodedIns.type) {
    case InstructionType::DataProcessing:
        decodeDataProcessing(rawIns, decodedIns);
        break;
    case InstructionType::Multiply:
        decodeMultiply(rawIns, decodedIns);
        break;
    case InstructionType::MultiplyLong:
        decodeMultiplyLong(rawIns, decodedIns);
        break;
    case InstructionType::SingleDataSwap:
        decodeSingleDataSwap(rawIns, decodedIns);
        break;
    case InstructionType::BranchAndExchange:
        decodeBranchAndExchange(rawIns, decodedIns);
        break;
    case InstructionType::HalfwordDataTransferReg:
        decodeHalfwordDataTransferReg(rawIns, decodedIns);
        break;
    case InstructionType::HalfwordDataTransferImm:
        decodeHalfwordDataTransferImm(rawIns, decodedIns);
        break;
    case InstructionType::SignedDataTransfer:
        decodeSignedDataTransfer(rawIns, decodedIns);
        break;
    case InstructionType::LoadStore:
        decodeLoadStore(rawIns, decodedIns);
        break;
    case InstructionType::BlockDataTransfer:
        decodeBlockDataTransfer(rawIns, decodedIns);
        break;
    case InstructionType::Branch:
        decodeBranch(rawIns, decodedIns);
        break;
    case InstructionType::SoftwareInterrupt:
        decodeSoftwareInterrupt(rawIns, decodedIns);
        break;
    case InstructionType::PSRTransfer:
        decodePSRTransfer(rawIns, decodedIns);
        break;
    case InstructionType::CoprocessorDataTransfer:
        decodeCoprocessorDataTransfer(rawIns, decodedIns);
        break;
    case InstructionType::CoprocessorDataOperation:
        decodeCoprocessorDataOperation(rawIns, decodedIns);
        break;
    case InstructionType::CoprocessorRegisterTransfer:
        decodeCoprocessorRegisterTransfer(rawIns, decodedIns);
        break;
    case InstructionType::Undefined:
    default:
        break;
    }

    return decodedIns;
}

void Decoder::decodeMultiply(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.W_A_bit = (rawIns >> 21) & 0b1; // A - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1; // S - bitNum: 20

    decIns.rd = (rawIns >> 16) & 0b1111; // Rd - bitNum: 19-16
    decIns.rn = (rawIns >> 12) & 0b1111; // Rn - bitNum: 15-12
    decIns.rs = (rawIns >> 8) & 0b1111;  // Rs - bitNum: 8-11
    decIns.rm = rawIns & 0b1111;         // Rm - bitNum: 3-0
}

void Decoder::decodeMultiplyLong(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // U - bitNum: 22
    decIns.W_A_bit = (rawIns >> 21) & 0b1;     // A - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1;     // S - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rd_MSW - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd_LSW - bitNum: 15-12
    decIns.rs = (rawIns >> 8) & 0b1111;  // Rn - bitNum: 11-8
    decIns.rm = rawIns & 0b1111;         // Rm - bitNum: 3-0
}

void Decoder::decodeBranchAndExchange(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.rn = rawIns & 0b1111; // Rn - bitNum: 3-0
}

void Decoder::decodeSingleDataSwap(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // B - bitNum: 22

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd - bitNum: 15-12
    decIns.rm = rawIns & 0b1111;         // Rm - bitNum: 3-0
}

void Decoder::decodeHalfwordDataTransferReg(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1; // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;   // U - bitNum: 23
    decIns.W_A_bit = (rawIns >> 21) & 0b1; // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1; // L - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd - bitNum: 15-12
    decIns.rm = rawIns & 0b1111;         // Rm - bitNum: 3-0
}

void Decoder::decodeHalfwordDataTransferImm(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1; // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;   // U - bitNum: 23
    decIns.W_A_bit = (rawIns >> 21) & 0b1; // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1; // L - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd - bitNum: 15-12

    uint32_t offset_left = ((rawIns >> 8) & 0b1111) << 4; // offset (left part) - bitNum: 11-8
    uint32_t offset_right = rawIns & 0b1111;              // offset (right part) - bitNum: 3-0

    decIns.offset = offset_left | offset_right; // offset - bitNum: 11-8 and 3-0
}

void Decoder::decodeSignedDataTransfer(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1;     // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;       // U - bitNum: 23
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // B - bitNum: 22
    decIns.W_A_bit = (rawIns >> 21) & 0b1;     // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1;     // L - bitNum: 20
    decIns.H_bit = (rawIns >> 5) & 0b1;        // H - bitNum: 5

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd - bitNum: 15-12

    if (decIns.B_U_N_S_bit == 1) {
        // Immediate offset
        uint32_t addr_left = ((rawIns >> 8) & 0b1111) << 4; // addr_mode (left part) - bitNum: 11-8
        uint32_t addr_right = rawIns & 0b1111;              // addr_mode (right part) - bitNum: 3-0
        decIns.offset = addr_left | addr_right;             // addr_mode - bitNum: 11-8 and 3-0
    } else {
        // Register offset
        decIns.rm = rawIns & 0b1111; // Rm - bitNum: 3-0
        decIns.offset = 0b0;
    }
}

void Decoder::decodeDataProcessing(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.S_L_bit = (rawIns >> 20) & 0b1; // S - bitNum: 20

    decIns.opcode = (rawIns >> 21) & 0b1111; // opcode - bitNum: 24-21
    decIns.rn = (rawIns >> 16) & 0b1111;     // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111;     // Rd - bitNum: 15-12

    decIns.operand2 = rawIns & 0xFFF; // operand2 - bitNum: 11-0
}

void Decoder::decodeLoadStore(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.I_bit = (rawIns >> 25) & 0b1;       // I - bitNum: 25
    decIns.P_L_bit = (rawIns >> 24) & 0b1;     // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;       // U - bitNum: 23
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // B - bitNum: 22
    decIns.W_A_bit = (rawIns >> 21) & 0b1;     // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1;     // L - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // Rd - bitNum: 15-12

    decIns.offset = rawIns & 0xFFF; // addr_mode - bitNum: 11-0
}

void Decoder::decodeBlockDataTransfer(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1;     // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;       // U - bitNum: 23
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // S - bitNum: 22
    decIns.W_A_bit = (rawIns >> 21) & 0b1;     // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1;     // L - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16

    decIns.register_list = rawIns & 0xFFFF; // register list - bitNum: 15-0
}

void Decoder::decodeBranch(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1; // L - bitNum: 24

    decIns.offset = rawIns & 0xFFFFFF; // offset - bitNum: 23-0
}

void Decoder::decodeCoprocessorDataTransfer(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.P_L_bit = (rawIns >> 24) & 0b1;     // P - bitNum: 24
    decIns.U_bit = (rawIns >> 23) & 0b1;       // U - bitNum: 23
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // N - bitNum: 22
    decIns.W_A_bit = (rawIns >> 21) & 0b1;     // W - bitNum: 21
    decIns.S_L_bit = (rawIns >> 20) & 0b1;     // L - bitNum: 20

    decIns.rn = (rawIns >> 16) & 0b1111; // Rn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111; // CRd - bitNum: 15-12
    decIns.rs = (rawIns >> 8) & 0b1111;  // CP# - bitNum: 11-8
    decIns.offset = rawIns & 0xFF;       // offset - bitNum: 7-0
}

void Decoder::decodeCoprocessorDataOperation(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.opcode = (rawIns >> 20) & 0b1111; // CP opcode - bitNum: 23-20
    decIns.rn = (rawIns >> 16) & 0b1111;     // CRn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111;     // CRd - bitNum: 15-12
    decIns.rs = (rawIns >> 8) & 0b1111;      // CP# - bitNum: 11-8
    decIns.cp = (rawIns >> 5) & 0b111;       // CP - bitNum: 7-5
    decIns.rm = rawIns & 0b1111;             // CRm - bitNum: 3-0
}

void Decoder::decodeCoprocessorRegisterTransfer(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.S_L_bit = (rawIns >> 20) & 0b1; // L - bitNum: 20

    decIns.opcode = (rawIns >> 21) & 0b111; // CP opc - bitNum: 23-21
    decIns.rn = (rawIns >> 16) & 0b1111;    // CRn - bitNum: 19-16
    decIns.rd = (rawIns >> 12) & 0b1111;    // Rd - bitNum: 15-12
    decIns.rs = (rawIns >> 8) & 0b1111;     // CP# - bitNum: 11-8
    decIns.cp = (rawIns >> 5) & 0b111;      // CP - bitNum: 7-5
    decIns.rm = rawIns & 0b1111;            // CRm - bitNum: 3-0
}

void Decoder::decodePSRTransfer(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.B_U_N_S_bit = (rawIns >> 22) & 0b1; // R - bitNum: 20
    decIns.I_bit = (rawIns >> 25) & 0b1;       // I - bitNum: 25

    decIns.opcode = (rawIns >> 21) & 0b1111; // opcode - bitNum: 24-21
    decIns.rd = (rawIns >> 12) & 0b1111;     // Rd - bitNum: 15-12

    decIns.operand2 = rawIns & 0xFFF; // operand2 - bitNum: 11-0
}

void Decoder::decodeSoftwareInterrupt(uint32_t rawIns, DecodedInstruction &decIns) {
    decIns.offset = rawIns & 0xFFFFFF; // offset - bitNum: 23-0
}

InstructionType Decoder::determineInsType(uint32_t rawIns) {
    uint32_t bits_27_26 = (rawIns >> 26) & 0b11;
    uint32_t bits_27_20 = (rawIns >> 20) & 0xFF; // 8 bits
    uint32_t bit_25 = (rawIns >> 25) & 0b1;
    uint32_t bits_7_4 = (rawIns >> 4) & 0b1111;
    uint32_t bits_7_6 = (rawIns >> 6) & 0b11;
    uint32_t bit_24 = (rawIns >> 24) & 0b1;
    uint32_t bit_23 = (rawIns >> 23) & 0b1;
    uint32_t bit_22 = (rawIns >> 22) & 0b1;
    uint32_t bit_4 = (rawIns >> 4) & 0b1;

    if (bits_27_26 == 0b00) {
        uint32_t opcode = (rawIns >> 21) & 0b1111;
        uint32_t s_bit = (rawIns >> 20) & 0b1;

        if (bit_25 == 0b1) {
            if (opcode == 0b1010 && s_bit == 0b0) {
                return InstructionType::PSRTransfer;
            }
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
            if ((opcode == 0b1000 || opcode == 0b1010) && s_bit == 0b0) {
                return InstructionType::PSRTransfer;
            }
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