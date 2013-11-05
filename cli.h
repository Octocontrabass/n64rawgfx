/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details. */

#include <stdint.h>

enum E_MODE { MODE_HELP, MODE_EXPORT, MODE_IMPORT };

#pragma pack(push, 1)
typedef struct {            // byte packing is mandatory
    uint16_t magic;         // 0x4D42, "BM"
    uint32_t filesize;      // offset + imagesize
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;        // 0x36
    uint32_t headersize;    // 0x28
    int32_t width;
    int32_t height;         // positive
    uint16_t planes;        // 1
    uint16_t bpp;           // 32
    uint32_t compression;   // 0
    uint32_t imagesize;     // width * height * 4 (bugged in nconvert)
    int32_t ppm_x;
    int32_t ppm_y;
    uint32_t clr_used;
    uint32_t clr_important;
} BMPHEADER;
#pragma pack(pop)
