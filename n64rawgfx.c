/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details. */

#include "n64rawgfx.h"

void n64_export( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint8_t *in, uint32_t *out, const uint32_t *pal )
{
    switch( format )
    {
        case FORMAT_RGBA:
        {
            switch( depth )
            {
                case DEPTH_16BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i] = ((in[i * 2 + 1] & 0x01)? 0xff000000 : 0x00000000)
                                | (in[i * 2] & 0xf8) << 16 | (in[i * 2] & 0xe0) << 11
                                | (in[i * 2] & 0x07) << 13 | (in[i * 2] & 0x07) << 8
                                | (in[i * 2 + 1] & 0xc0) << 5
                                | (in[i * 2 + 1] & 0x3e) << 2 | (in[i * 2 + 1] & 0x38) >> 3;
                    }
                    break;
                }
                case DEPTH_32BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i] = in[i * 4 + 3] << 24 | in[i * 4] << 16 | in[i * 4 + 1] << 8 | in[i * 4 + 2];
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case FORMAT_YUV:
        {
            break;
        }
        case FORMAT_CI:
        {
            switch( depth )
            {
                case DEPTH_4BIT:
                {
                    for( size_t i = 0; i < count; i+=2 )
                    {
                        out[i] = pal[in[i / 2] >> 4];
                        out[i + 1] = pal[in[i / 2] & 0xf];
                    }
                    break;
                }
                case DEPTH_8BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i] = pal[in[i]];
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case FORMAT_IA:
        {
            switch( depth )
            {
                case DEPTH_16BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i] = in[i * 2 + 1] << 24 | in[i * 2] << 16 | in[i * 2] << 8 | in[i * 2];
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case FORMAT_I:
        {
            switch( depth )
            {
                case DEPTH_4BIT:
                {
                    for( size_t i = 0; i < count; i+=2 )
                    {
                        out[i] = (in[i / 2] & 0xf0) << 16 | (in[i / 2] & 0xf0) << 12
                                | (in[i / 2] & 0xf0) << 8 | (in[i / 2] & 0xf0) << 4
                                | (in[i / 2] & 0xf0) | (in[i / 2] & 0xf0) >> 4;
                        out[i + 1] = (in[i / 2] & 0x0f) << 20 | (in[i / 2] & 0x0f) << 16
                                | (in[i / 2] & 0x0f) << 12 | (in[i / 2] & 0x0f) << 8
                                | (in[i / 2] & 0x0f) << 4 | (in[i / 2] & 0x0f);
                    }
                    break;
                }
                case DEPTH_8BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i] = in[i] << 16 | in[i] << 8 | in[i];
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
    }
    return;
}

void n64_import( enum E_FORMAT format, enum E_DEPTH depth, size_t count, const uint32_t *in, uint8_t *out )
{
    switch( format )
    {
        case FORMAT_RGBA:
        {
            switch( depth )
            {
                case DEPTH_16BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i * 2] = (in[i] & 0xf80000) >> 16 | (in[i] & 0xe000) >> 13;
                        out[i * 2 + 1] = (in[i] & 0x1800) >> 5 | (in[i] & 0xf8) >> 2 | (in[i] & 0x80000000) >> 31;
                    }
                    break;
                }
                case DEPTH_32BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        out[i * 4] = (in[i] & 0xff0000) >> 16;
                        out[i * 4 + 1] = (in[i] & 0xff00) >> 8;
                        out[i * 4 + 2] = in[i] & 0xff;
                        out[i * 4 + 3] = (in[i] & 0xff000000) >> 24;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case FORMAT_YUV:
        {
            break;
        }
        case FORMAT_CI:
        {
            break;
        }
        case FORMAT_IA:
        {
            switch( depth )
            {
                case DEPTH_16BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        uint32_t temp = ((in[i] & 0xff0000) >> 16) + ((in[i] & 0xff00) >> 7) + (in[i] & 0xff);
                        out[i * 2] = (temp + (temp >> 8)) >> 2;
                        out[i * 2 + 1] = (in[i] & 0xff000000) >> 24;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case FORMAT_I:
        {
            switch( depth )
            {
                case DEPTH_4BIT:
                {
                    for( size_t i = 0; i < count; i+=2 )
                    {
                        uint32_t temp1 = ((in[i] & 0xff0000) >> 16) + ((in[i] & 0xff00) >> 7) + (in[i] & 0xff);
                        uint32_t temp2 = ((in[i + 1] & 0xff0000) >> 16) + ((in[i + 1] & 0xff00) >> 7) + (in[i + 1] & 0xff);
                        out[i / 2] = ((temp1 + (temp1 >> 8)) & 0x3c0) >> 2 | ((temp2 + (temp2 >> 8)) & 0x3c0) >> 6;
                    }
                    break;
                }
                case DEPTH_8BIT:
                {
                    for( size_t i = 0; i < count; i++ )
                    {
                        uint32_t temp = ((in[i] & 0xff0000) >> 16) + ((in[i] & 0xff00) >> 7) + (in[i] & 0xff);
                        out[i] = (temp + (temp >> 8)) >> 2;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}
