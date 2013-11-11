N64 Raw Graphics Tool
=====================

A tool to export and import uncompressed (a.k.a. "raw") graphics from Nintendo 64 games.

Download
--------

[Download for Windows][1]. (Also available for [64-bit Windows][2].)

Usage
-----

It has built-in help, so look at that to see an explanation of all the parameters. As an example, the following command exports the [cactus][3] from an extended Super Mario 64 ROM.

    n64rawgfx -m export -r "Super Mario 64.ext.z64" -b cactus.bmp -f RGBA -d 16 -a 0xcdbbd1 -x 32 -y 32

Importing is similar, except the image dimensions are automatically read from the BMP file. The following command replaces the coins with cacti.

    n64rawgfx -m import -r "Super Mario 64.ext.z64" -b cactus.bmp -f IA -d 16 -a 0xAB7B8C

If you don't specify the BMP filename during import or export, the address (padded to eight digits) will be used as the filename.

Export Formats
--------------

The following formats can be exported:

* RGBA (16-bit, 32-bit)
* CI (4-bit, 8-bit)
* IA (16-bit)
* I (4-bit, 8-bit)

The output file will be a 32-bit BMP file with an alpha channel.

Import Formats
--------------

The following formats can be imported:

* RGBA (16-bit, 32-bit)
* IA (16-bit)
* I (4-bit, 8-bit)

The input file must be a 32-bit BMP file.

[1]: http://derpa.no-ip.org/b/n64rawgfx.zip "Windows"
[2]: http://derpa.no-ip.org/b/n64rawgfx64.zip "Windows 64-bit"
[3]: http://tcrf.net/File:Cactus.png "Cactus!"
