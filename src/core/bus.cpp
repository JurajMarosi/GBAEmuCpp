#include "core/bus.hpp"

#include <algorithm>

using namespace std;

Bus::Bus() {
    fill(ewram, ewram + ewramSize, 0);
    fill(iwram, iwram + iwramSize, 0);
    fill(vram, vram + vramSize, 0);
}

Bus::~Bus() {}

uint8_t Bus::read8(uint32_t memAddr) {
    uint8_t region = memAddr >> 24;

    switch (region) {
    case 0x02: // EWRAM
        return ewram[memAddr - 0x02000000];

    case 0x03: // IWRAM
        return iwram[(memAddr - 0x03000000) & 0x7FFF];

    case 0x06: // VRAM
        return vram[memAddr - 0x06000000];

    default:
        return 0;
    }
}

uint16_t Bus::read16(uint32_t memAddr) {
    return read8(memAddr) | (read8(memAddr + 1) << 8);
}

uint32_t Bus::read32(uint32_t memAddr) {
    return read8(memAddr) | (read8(memAddr + 1) << 8) |
           (read8(memAddr + 2) << 16) | (read8(memAddr + 3) << 24);
}

void Bus::write8(uint32_t memAddr, uint8_t val) {
    uint8_t region = memAddr >> 24;

    switch (region) {
    case 0x02: // EWRAM
        ewram[memAddr - 0x02000000] = val;
        break;

    case 0x03: // IWRAM
        iwram[(memAddr - 0x03000000) & 0x7FFF] = val;
        break;

    case 0x06: // VRAM
        vram[memAddr - 0x06000000] = val;
        break;

    default:
        return;
    }
}

void Bus::write16(uint32_t memAddr, uint16_t val) {
    write8(memAddr, val & 0xFF);
    write8(memAddr + 1, (val >> 8) & 0xFF);
}

void Bus::write32(uint32_t memAddr, uint32_t val) {
    write8(memAddr, val & 0xFF);
    write8(memAddr + 1, (val >> 8) & 0xFF);
    write8(memAddr + 2, (val >> 16) & 0xFF);
    write8(memAddr + 3, (val >> 24) & 0xFF);
}
