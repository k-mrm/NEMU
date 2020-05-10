#include <stdlib.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"

Tile *new_tile() {
    Tile *t = malloc(sizeof(Tile));
    return t;
}

Tile *ppu_make_pixelpat(PPU *ppu, uint16_t sid) {
    uint8_t spr[16];
    uint16_t idx = sid * 16;
    for(int i = 0; i < 16; ++i, ++idx) {
        spr[i] = ppubus_read(ppu->bus, idx);
    }

    Tile *tile = new_tile();
    for(int i = 0; i < 16; ++i) {
        for(int j = 0; j < 8; ++j) {
            tile->pp[i % 8][j] |= ((spr[i] & (0x80 >> j)) != 0) << (i / 8);
        }
    }

    return tile;
}
