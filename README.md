# disarmv4t
An ARMv4T disassembler.

## Usage
```
usage:
  disarmv4t [--base <value>] [--thumb] [--format <value>] <input> <output>

keyword arguments:
  -b, --base      Base address (default: 0)
  -t, --thumb     Disassemble as Thumb (default: false)
  -f, --format    Output format (default: {addr:08X}  {instr:08X}  {mnemonic})

positional arguments:
  input     Input file
  output    Output file
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

Running `disarmv4t --base 0x8000000 in.bin out.txt` generates:

```
08000000  E3A00C01  mov       r0,0x100
08000004  E3A01302  mov       r1,0x8000000
08000008  E3A02403  mov       r2,0x3000000
0800000C  E4914004  ldr       r4,[r1],0x4
08000010  E4824004  str       r4,[r2],0x4
08000014  E2500001  subs      r0,r0,0x1
08000018  1AFFFFFB  bne       0x800000C
```

## Binaries
Binaries for Windows, Linux and macOS are available as [nightly](https://github.com/jsmolka/disarmv4t/actions) or [release](https://github.com/jsmolka/disarmv4t/releases) builds.

## Build
Detailed build instructions can be found [here](BUILD.md).
