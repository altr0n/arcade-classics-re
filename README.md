# arcade-classics-reversing

## What
* Reverse Engineering the Basic Fun arcade cabinets which can be purchsed [here](https://www.amazon.com/Qbert-Classic-Mini-Arcade-Game/dp/B06XSMSG5Q/ref=sr_1_1?srs=2582955011&ie=UTF8&qid=1511545572&sr=8-1&keywords=QBert+arcade)

## Tools
* An arduino based tool was written to dump the SPI flash over the serial port
* A python client was developed to receive and reconstruct the resulting flash image
* Eventually a Raspberry Pi was used in conjunction with flashrom to quickly extract and rewrite the flash for testing

## Hardware
* The Basic Fun cabinets consist of what appears to be a "NES on a Chip" which reads the ROM file from the SPI flash
* FLash is broken up into identical chunks of 0x80000
  * Init Code 0:0x4000
  * Possible CHR Table 0x4000:0x6000
  * Erased 0x6000-0x8000
  * QBERT 0x8000:0x18000
  * ROM At 0x70000
  * Possible code at 0x73700
   * Could this be the Test ROM?
  * Possible code at 0x73b00 < -- This is likely a ROM
  * Possible code at 0x75800:75A80
    * Note: When we NOP: This is the CHR table for the test ROM!
  * ROM List starts at 0x7C000
  * Seems to be more 6502 code at 0x7D100
* Bootloader seems to be located at 0x7D100, if this ia modified then the cabinet will not operate
  * It looks like this gets mapped into memory potentially at 0xD100?
  * 0x7D100 - 0x7F900
* SPI Pinout
 * ![SPI Pinout](https://i.imgur.com/3yCf6k4.jpg)
* Generic SPI Pinout
 * ![Generic SPI](https://www.arduino.cc/en/uploads/Tutorial/at25hp512_pins2.jpg)
## Software
* The software (ROMS) present on the cabinets are all located within the SPI flash
* The ROMs appear to be almost identical to the original NES Roms with multiple modifications made
 * All branches which would be taken to enable multiplayer functionality have been NOPed out
 * Copyright strings and manufacturer strings have been replaced

## Modifications
* The ROM can be replaces with another NES ROM, and while the SoC seems to run it properly, the CHR tables are broken somewhere, this will take further reversing
* Example 1:
 * ![Tetris Running on Cab](https://i.imgur.com/1w1UAbx.jpg)

