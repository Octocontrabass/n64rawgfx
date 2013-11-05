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

Importing doesn't work yet, but when it does, the command will be similar.

    n64rawgfx -m import -r "Super Mario 64.ext.z64" -b cactus.bmp -f IA -d 16 -a 0xAB7B8C

Export Formats
--------------

The following formats can be exported:

* RGBA (16-bit, 32-bit)
* IA (16-bit)
* I (4-bit, 8-bit)

The output file will be a 32-bit BMP file with an alpha channel.

Import Formats
------------------------

Importing doesn't work yet. When it does, it should accept any 32-bit BMP with an alpha channel.

[1]: http://derpa.no-ip.org/b/n64rawgfx.zip "Windows"
[2]: http://derpa.no-ip.org/b/n64rawgfx64.zip "Windows 64-bit"
[3]: http://tcrf.net/File:Cactus.png "Cactus!"
