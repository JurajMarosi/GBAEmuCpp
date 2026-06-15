#pragma once

#include <cstdint>

class Bus {
  private:
    static const int ewramSize = 256 * 1024;
    static const int iwramSize = 32 * 1024;
    static const int vramSize = 96 * 1024;

    uint8_t ewram[ewramSize];
    uint8_t iwram[iwramSize];
    uint8_t vram[vramSize];

  public:
    Bus();
    ~Bus();

    uint8_t read8(uint32_t memAddr);
    uint16_t read16(uint32_t memAddr);
    uint32_t read32(uint32_t memAddr);

    void write8(uint32_t memAddr, uint8_t val);
    void write16(uint32_t memAddr, uint16_t val);
    void write32(uint32_t memAddr, uint32_t val);
};
