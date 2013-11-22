/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details. */

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cli.h"
#include "n64rawgfx.h"

void __attribute__((noreturn)) print_help( const char* const name )
{
    fprintf( stderr,
        "N64 Raw Graphics Tool\n"
        "Usage: %s [options]\n"
        "Options:\n"
        "  -h         --help             Show this help\n"
        "  -r <file>  --romfile <file>   Export from/import to ROM file\n"
        "  -b <file>  --bmpfile <file>   Export to/import from BMP file\n"
        "  -m <mode>  --mode <mode>      Mode (export, import)\n"
        "  -f <fmt>   --format <fmt>     Format (RGBA, CI, IA, I)\n"
//      "  -f <fmt>   --format <fmt>     Format (RGBA, YUV, CI, IA, I)\n"
        "  -d <bits>  --depth <bits>     Bit depth (4, 8, 16, 32)\n"
        "  -a <addr>  --address <addr>   Address (use \"0x\" for hexadecimal)\n"
        "  -x <num>   --width <num>      Width (export only)\n"
        "  -y <num>   --height <num>     Height (export only)\n"
        "             --pdepth <bits>    Palette depth (16, 32) (CI only)\n"
        "             --paddress <addr>  Palette address (CI only)\n"
        "\n"
        "https://github.com/Octocontrabass\n", name );
    exit( EXIT_SUCCESS );
}

void *checked_malloc( size_t size )
{
    void *ret = malloc( size );
    if( ret == NULL )
    {
        fprintf( stderr, "Out of memory!\n" );
        exit( EXIT_FAILURE );
    }
    return ret;
}

int main( int argc, char **argv )
{
    char *romname = NULL;
    char *bmpname = NULL;
    enum E_MODE mode = MODE_HELP;
    enum E_FORMAT format = -1;
    enum E_DEPTH depth = -1;
    enum E_DEPTH pdepth = -1;
    long address = -1;
    long paddress = -1;
    int32_t width = 0;
    int32_t height = 0;
    FILE *romfile = NULL;
    FILE *bmpfile = NULL;
    
    while(1)
    {
        struct option longopts[] = {
            { "help",     no_argument,       0, 'h' },
            { "romfile",  required_argument, 0, 'r' },
            { "bmpfile",  required_argument, 0, 'b' },
            { "mode",     required_argument, 0, 'm' },
            { "format",   required_argument, 0, 'f' },
            { "depth",    required_argument, 0, 'd' },
            { "address",  required_argument, 0, 'a' },
            { "width",    required_argument, 0, 'x' },
            { "height",   required_argument, 0, 'y' },
            { "pdepth",   required_argument, 0, 'e' },
            { "paddress", required_argument, 0, 'z' },
            { 0,         0,                 0, 0   }
        };
        int opt = getopt_long( argc, argv, "hr:b:m:f:d:a:x:y:", longopts, NULL );
        if( opt == -1 )
        {
            break;
        }
        switch( opt )
        {
            case 'r':
                romname = optarg;
                break;
            case 'b':
                bmpname = optarg;
                break;
            case 'm':
                if( strncasecmp( optarg, "e", 1 ) == 0 )
                {
                    mode = MODE_EXPORT;
                }
                else if( strncasecmp( optarg, "i", 1 ) == 0 )
                {
                    mode = MODE_IMPORT;
                }
                break;
            case 'f':
                if( strcasecmp( optarg, "RGBA" ) == 0 )
                {
                    format = FORMAT_RGBA;
                }
                else if( strcasecmp( optarg, "YUV" ) == 0 )
                {
                    format = FORMAT_YUV;
                }
                else if( strcasecmp( optarg, "CI" ) == 0 )
                {
                    format = FORMAT_CI;
                }
                else if( strcasecmp( optarg, "IA" ) == 0 )
                {
                    format = FORMAT_IA;
                }
                else if( strcasecmp( optarg, "I" ) == 0 )
                {
                    format = FORMAT_I;
                }
                break;
            case 'd':
            {
                long temp = strtol( optarg, NULL, 0 );
                switch( temp )
                {
                    case 4:
                        depth = DEPTH_4BIT;
                        break;
                    case 8:
                        depth = DEPTH_8BIT;
                        break;
                    case 16:
                        depth = DEPTH_16BIT;
                        break;
                    case 32:
                        depth = DEPTH_32BIT;
                        break;
                }
                break;
            }
            case 'e':
            {
                long temp = strtol( optarg, NULL, 0 );
                switch( temp )
                {
                    case 16:
                        pdepth = DEPTH_16BIT;
                        break;
                    case 32:
                        pdepth = DEPTH_32BIT;
                        break;
                }
                break;
            }
            case 'a':
                address = strtol( optarg, NULL, 0 );
                break;
            case 'z':
                paddress = strtol( optarg, NULL, 0 );
                break;
            case 'x':
                width = strtol( optarg, NULL, 0 );
                break;
            case 'y':
                height = strtol( optarg, NULL, 0 );
                break;
            default:
                print_help( argv[0] );
                break;
        }
    }
    
    switch( mode )
    {
        case MODE_EXPORT:
        {
            BMPHEADER header = {0x4D42,0,0,0,0x36,0x28,0,0,1,32,0,0,0,0,0,0};
            size_t size;
            uint8_t *ibuf;
            uint32_t *obuf;
            uint32_t *pbuf = NULL;
            
            if( romname == NULL || format < 0 || depth < 0 || address < 0 || width <= 0 || height <= 0 )
            {
                fprintf( stderr, "Invalid arguments for export.\n" );
                return EXIT_FAILURE;
            }
            switch( format )
            {
                case FORMAT_RGBA:
                {
                    if( depth < DEPTH_16BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                case FORMAT_CI:
                {
                    if( pdepth < 0 || paddress < 0 )
                    {
                        fprintf( stderr, "Invalid arguments for export.\n" );
                        return EXIT_FAILURE;
                    }
                    if( depth > DEPTH_8BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                case FORMAT_IA:
                {
                    if( depth > DEPTH_16BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                case FORMAT_I:
                {
                    if( depth > DEPTH_8BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                default:
                {
                    fprintf( stderr, "Unsupported format.\n" );
                    return EXIT_FAILURE;
                }
            }
            romfile = fopen( romname, "rb" );
            if( romfile == NULL )
            {
                fprintf( stderr, "Could not open %s for reading.\n", romname );
                return EXIT_FAILURE;
            }
            if( bmpname == NULL )
            {
                if( address > UINT32_MAX )
                {
                    fprintf( stderr, "Invalid arguments for export.\n" );
                    return EXIT_FAILURE;
                }
                bmpname = checked_malloc( 13 );
                sprintf( bmpname, "%08" PRIX32 ".bmp", (uint32_t)address );
                bmpfile = fopen( bmpname, "wb" );
                if( bmpfile == NULL )
                {
                    fprintf( stderr, "Could not open %s for writing.\n", bmpname );
                    return EXIT_FAILURE;
                }
                free( bmpname );
            }
            else
            {
                bmpfile = fopen( bmpname, "wb" );
                if( bmpfile == NULL )
                {
                    fprintf( stderr, "Could not open %s for writing.\n", bmpname );
                    return EXIT_FAILURE;
                }
            }
            
            if( format == FORMAT_CI )
            {
                if( fseek( romfile, paddress, SEEK_SET ) )
                {
                    fprintf( stderr, "Failed to read input file.\n" );
                    return EXIT_FAILURE;
                }
                int colors = (depth == DEPTH_4BIT)? 16 : 256;
                size = colors * ((pdepth == DEPTH_16BIT)? 2 : 4);
                ibuf = checked_malloc( size );
                pbuf = checked_malloc( colors * sizeof( uint32_t ) );
                if( fread( ibuf, 1, size, romfile ) != size )
                {
                    fprintf( stderr, "Failed to read input file.\n" );
                    return EXIT_FAILURE;
                }
                n64_export( FORMAT_RGBA, pdepth, colors, ibuf, pbuf, NULL );
                free( ibuf );
            }
            
            if( fseek( romfile, address, SEEK_SET ) )
            {
                fprintf( stderr, "Failed to read input file.\n" );
                return EXIT_FAILURE;
            }
            
            if( depth == DEPTH_4BIT && (width & 1) > 0 ) width++;
            
            header.width = width;
            header.height = height;
            header.imagesize = width * height * 4;
            header.filesize = header.offset + header.imagesize;
            
            switch( depth )
            {
                case DEPTH_4BIT:
                    size = (width / 2) * height;
                    break;
                case DEPTH_8BIT:
                    size = width * height;
                    break;
                case DEPTH_16BIT:
                    size = width * height * 2;
                    break;
                default:
                    size = width * height * 4;
                    break;
            }
            
            ibuf = checked_malloc( size );
            obuf = checked_malloc( header.imagesize );
            
            if( fread( ibuf, 1, size, romfile ) != size )
            {
                fprintf( stderr, "Failed to read input file.\n" );
                return EXIT_FAILURE;
            }
            n64_export( format, depth, width * height, ibuf, obuf, pbuf );
            fwrite( &header, sizeof( BMPHEADER ), 1, bmpfile );
            for( int32_t y = height - 1; y >= 0; y-- )
            {
                fwrite( obuf + (y * width), sizeof( uint32_t ), width, bmpfile );
            }
            fclose( romfile );
            fclose( bmpfile );
            
            return EXIT_SUCCESS;
        }
        case MODE_IMPORT:
        {
            BMPHEADER header;
            size_t size;
            uint32_t *ibuf;
            uint8_t *obuf;
            
            if( romname == NULL || format < 0 || depth < 0 || address < 0 )
            {
                fprintf( stderr, "Invalid arguments for import.\n" );
                return EXIT_FAILURE;
            }
            switch( format )
            {
                case FORMAT_RGBA:
                {
                    if( depth < DEPTH_16BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                case FORMAT_IA:
                {
                    if( depth > DEPTH_16BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                case FORMAT_I:
                {
                    if( depth > DEPTH_8BIT )
                    {
                        fprintf( stderr, "Unsupported format.\n" );
                        return EXIT_FAILURE;
                    }
                    break;
                }
                default:
                {
                    fprintf( stderr, "Unsupported format.\n" );
                    return EXIT_FAILURE;
                }
            }
            romfile = fopen( romname, "r+b" );
            if( romfile == NULL )
            {
                fprintf( stderr, "Could not open %s for writing.\n", romname );
                return EXIT_FAILURE;
            }
            if( bmpname == NULL )
            {
                if( address > UINT32_MAX )
                {
                    fprintf( stderr, "Invalid arguments for import.\n" );
                    return EXIT_FAILURE;
                }
                bmpname = checked_malloc( 13 );
                sprintf( bmpname, "%08" PRIX32 ".bmp", (uint32_t)address );
                bmpfile = fopen( bmpname, "rb" );
                if( bmpfile == NULL )
                {
                    fprintf( stderr, "Could not open %s for reading.\n", bmpname );
                    return EXIT_FAILURE;
                }
                free( bmpname );
            }
            else
            {
                bmpfile = fopen( bmpname, "rb" );
                if( bmpfile == NULL )
                {
                    fprintf( stderr, "Could not open %s for reading.\n", bmpname );
                    return EXIT_FAILURE;
                }
            }
            
            if( fread( &header, sizeof( BMPHEADER ), 1, bmpfile ) != 1 )
            {
                fprintf( stderr, "Input file invalid.\n" );
                return EXIT_FAILURE;
            }
            if( header.magic != 0x4d42 || header.headersize < 0x28
                || header.width < 1 || header.height < 1
                || header.planes != 1 || header.bpp != 32
                || header.compression != 0 )
            {
                fprintf( stderr, "Input file unsupported or invalid.\n" );
                return EXIT_FAILURE;
            }
            
            if( fseek( bmpfile, header.offset, SEEK_SET ) )
            {
                fprintf( stderr, "Failed to read input file.\n" );
                return EXIT_FAILURE;
            }
            if( fseek( romfile, address, SEEK_SET ) )
            {
                fprintf( stderr, "Failed to read output file.\n" );
                return EXIT_FAILURE;
            }
            
            width = header.width;
            height = header.height;
            
            if( depth == DEPTH_4BIT && (width & 1) > 0 )
            {
                fprintf( stderr, "Width must be divisible by 2 for 4-bit.\n" );
                return EXIT_FAILURE;
            }
            
            switch( depth )
            {
                case DEPTH_4BIT:
                    size = (width / 2) * height;
                    break;
                case DEPTH_8BIT:
                    size = width * height;
                    break;
                case DEPTH_16BIT:
                    size = width * height * 2;
                    break;
                default:
                    size = width * height * 4;
                    break;
            }
            
            ibuf = checked_malloc( width * height * 4 );
            obuf = checked_malloc( size );
            
            for( int32_t y = height - 1; y >= 0; y-- )
            {
                if( fread( ibuf + (y * width), sizeof( uint32_t ), width, bmpfile ) != width )
                {
                    fprintf( stderr, "Error reading bitmap file.\n" );
                    return EXIT_FAILURE;
                }
            }
            n64_import( format, depth, width * height, ibuf, obuf );
            fwrite( obuf, 1, size, romfile );
            fclose( romfile );
            fclose( bmpfile );
            
            return EXIT_SUCCESS;
        }
        default:
            print_help( argv[0] );
    }
}

