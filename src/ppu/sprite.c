#include <stdio.h>
#include "ppu/ppu.h"
#include "ppu/bus.h"
#include "ppu/sprite.h"

void ppu_sprite_dump(PPU *ppu, uint16_t sid) {
    uint8_t spr[16];
    uint16_t idx = sid * 16;
    for(int i = 0; i < 16; ++i, ++idx) {
        spr[i] = ppubus_read(ppu->bus, idx);
    }

    uint8_t tile[8][8] = {{0}};
    for(int i = 0; i < 16; ++i) {
        for(int j = 0; j < 8; ++j) {
            tile[i % 8][j] |= ((spr[i] & (0x80 >> j)) != 0) << (i / 8);
        }
    }

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            switch(tile[i][j]) {
            case 0: printf("  "); break;
            case 1: printf(". "); break;
            case 2: printf("* "); break;
            case 3: printf("# "); break;
            default:printf("%d", tile[i][j]);
            }
        }
        puts("");
    }
}
