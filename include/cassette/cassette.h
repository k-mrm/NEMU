#ifndef NEMU_CASSETTE_H
#define NEMU_CASSETTE_H

#define HEADER_BYTE 16

typedef struct Cassette Cassette;
struct Cassette {
    size_t nprgrom_byte;
    size_t nchrrom_byte;
    unsigned char *prgrom;
    unsigned char *chrrom;
};

extern Cassette cassette;

int read_cassette(const char *);


#endif
