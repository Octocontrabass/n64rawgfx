/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details. */

/* Each format is marked to indicate its level of support. CI formats
 * require a palette to be provided.
 * 
 *  Format  4-bit   8-bit   16-bit  32-bit
 *  RGBA    ------  ------  YES     YES
 *  YUV     ------  ------  no      ------
 *  CI      export  export  ------  ------
 *  IA      export  export  YES     ------
 *  I       YES     YES     ------  ------
 */

#include <stdint.h>
#include <stdlib.h>

enum E_FORMAT { FORMAT_RGBA, FORMAT_YUV, FORMAT_CI, FORMAT_IA, FORMAT_I };
enum E_DEPTH { DEPTH_4BIT, DEPTH_8BIT, DEPTH_16BIT, DEPTH_32BIT };

void n64_export( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint8_t *in, uint32_t *out, const uint32_t *pal );
void n64_import( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint32_t *in, uint8_t *out );
