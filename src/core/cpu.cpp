#include "core/cpu.hpp"

using namespace std;

CPU::CPU(Bus &bus) : memBus(bus), cpsr(0x1F) {
    Decoder::initLUT();

    for (int i = 0; i < 16; i++) {
        regs[i] = 0;
    }

    executeLut[static_cast<int>(InstructionType::Multiply)] = &CPU::executeMultiply;
    executeLut[static_cast<int>(InstructionType::MultiplyLong)] = &CPU::executeMultiplyLong;
    executeLut[static_cast<int>(InstructionType::BranchAndExchange)] = &CPU::executeBranchAndExchange;
    executeLut[static_cast<int>(InstructionType::SingleDataSwap)] = &CPU::executeSingleDataSwap;
    executeLut[static_cast<int>(InstructionType::HalfwordDataTransferReg)] = &CPU::executeHalfwordDataTransferReg;
    executeLut[static_cast<int>(InstructionType::HalfwordDataTransferImm)] = &CPU::executeHalfwordDataTransferImm;
    executeLut[static_cast<int>(InstructionType::SignedDataTransfer)] = &CPU::executeSignedDataTransfer;
    executeLut[static_cast<int>(InstructionType::DataProcessing)] = &CPU::executeDataProcessing;
    executeLut[static_cast<int>(InstructionType::LoadStore)] = &CPU::executeLoadStore;
    executeLut[static_cast<int>(InstructionType::Undefined)] = &CPU::executeUndefined;
    executeLut[static_cast<int>(InstructionType::BlockDataTransfer)] = &CPU::executeBlockDataTransfer;
    executeLut[static_cast<int>(InstructionType::Branch)] = &CPU::executeBranch;
    executeLut[static_cast<int>(InstructionType::CoprocessorDataTransfer)] = &CPU::executeCoprocessorDataTransfer;
    executeLut[static_cast<int>(InstructionType::CoprocessorDataOperation)] = &CPU::executeCoprocessorDataOperation;
    executeLut[static_cast<int>(InstructionType::CoprocessorRegisterTransfer)] = &CPU::executeCoprocessorRegisterTransfer;
    executeLut[static_cast<int>(InstructionType::PSRTransfer)] = &CPU::executePSRTransfer;
    executeLut[static_cast<int>(InstructionType::SoftwareInterrupt)] = &CPU::executeSoftwareInterrupt;
}

CPU::~CPU() {}

void CPU::fetch() {
    currentIns = memBus.read32(regs[PC]);
    regs[PC] += 4;
}

void CPU::decode() { decodedIns = decoder.decode(currentIns); }

void CPU::execute() { (this->*executeLut[static_cast<int>(decodedIns.type)])(decodedIns); }

void CPU::step() {
    fetch();
    decode();
    execute();
}

void CPU::executeMultiply(const DecodedInstruction &decIns) {}
void CPU::executeMultiplyLong(const DecodedInstruction &decIns) {}
void CPU::executeBranchAndExchange(const DecodedInstruction &decIns) {}
void CPU::executeSingleDataSwap(const DecodedInstruction &decIns) {}
void CPU::executeHalfwordDataTransferReg(const DecodedInstruction &decIns) {}
void CPU::executeHalfwordDataTransferImm(const DecodedInstruction &decIns) {}
void CPU::executeSignedDataTransfer(const DecodedInstruction &decIns) {}
void CPU::executeDataProcessing(const DecodedInstruction &decIns) {}
void CPU::executeLoadStore(const DecodedInstruction &decIns) {}
void CPU::executeUndefined(const DecodedInstruction &decIns) {}
void CPU::executeBlockDataTransfer(const DecodedInstruction &decIns) {}
void CPU::executeBranch(const DecodedInstruction &decIns) {}
void CPU::executeCoprocessorDataTransfer(const DecodedInstruction &decIns) {}
void CPU::executeCoprocessorDataOperation(const DecodedInstruction &decIns) {}
void CPU::executeCoprocessorRegisterTransfer(const DecodedInstruction &decIns) {}
void CPU::executePSRTransfer(const DecodedInstruction &decIns) {}
void CPU::executeSoftwareInterrupt(const DecodedInstruction &decIns) {}
