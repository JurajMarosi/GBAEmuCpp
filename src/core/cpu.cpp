#include "core/cpu.hpp"

using namespace std;

CPU::CPU(Bus &bus) {}
CPU::~CPU() {}

void CPU::fetch() {
    currentIns = bus.read32(regs[PC]);
    regs[PC] += 4;
}

void CPU::decode() {}

void CPU::execute() {}

void CPU::step() {
    fetch();
    decode();
    execute();
}