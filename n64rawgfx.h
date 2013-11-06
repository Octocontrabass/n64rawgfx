/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details. */

/* Formats that are known to exist are marked with their level of
 * support. Formats that are impossible are crossed out. Unknown formats
 * have question marks.
 * 
 *  Format  4-bit   8-bit   16-bit  32-bit
 *  RGBA    ------  ------  export  export
 *  YUV     ?       ?       no      ?
 *  CI      no      no      ------  ------
 *  IA      ?       ?       export  ?
 *  I       export  export  ------  ------
 */

#include <stdint.h>
#include <stdlib.h>

enum E_FORMAT { FORMAT_RGBA, FORMAT_YUV, FORMAT_CI, FORMAT_IA, FORMAT_I };
enum E_DEPTH { DEPTH_4BIT, DEPTH_8BIT, DEPTH_16BIT, DEPTH_32BIT };

void n64_export( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint8_t *in, uint32_t *out );
void n64_import( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint32_t *in, uint8_t *out );
