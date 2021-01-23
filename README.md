# disarmv4t
An ARMv4T disassembler.

## Usage
```
Usage: disarmv4t [--help] [--base <value>] [--thumb] [--format <value>] <input> <output>
```

## Example
```
main:
        mov     r0, 0x100
        mov     r1, 0x8000000
        mov     r2, 0x3000000
loop:
        ldr     r4, [r1], 4
        str     r4, [r2], 4
        subs    r0, 1
        bne     loop
```

Executing `disarmv4t in.bin out.txt` generates the following output:

```
00000000  E3A00C01  mov       r0,0x100
00000004  E3A01302  mov       r1,0x8000000
00000008  E3A02403  mov       r2,0x3000000
0000000C  E4914004  ldr       r4,[r1],0x4
00000010  E4824004  str       r4,[r2],0x4
00000014  E2500001  subs      r0,r0,0x1
00000018  1AFFFFFB  bne       0xC
```

## Binaries
Binaries for Windows, Linux and macOS are built on each commit and can be found in the [Actions](https://github.com/jsmolka/disarmv4t/actions) tab on GitHub.

## Building
Detailed build instructions can be found [here](BUILDING.md).
