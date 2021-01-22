#include <shell/bit.h>
#include <shell/filesystem.h>
#include <shell/fmt.h>
#include <shell/int.h>
#include <shell/main.h>
#include <shell/options.h>

#include "disassemble.h"

namespace fs = shell::filesystem;

using namespace shell;

u32 parseBase(std::string data)
{
    std::string unmodified = data;

    int base = 10;

    shell::toLower(data);

    if (data.find("0b") == 0)
    {
        base = 2;
        replaceFirst(data, "0b", "");
    }
    if (data.find("0x") == 0)
    {
        base = 16;
        replaceFirst(data, "0x", "");
    }

    if (auto result = shell::parseInt<u32>(data, base))
    {
        return *result;
    }
    
    throw FormattedError("Cannot parse base {}", unmodified);
}

int main(int argc, char* argv[])
{
    Options options("disarmv4t");
    options.add({   "--help,-h", "Show help"              }, Options::value<bool>());
    options.add({   "--base,-b", "Base address", "value"  }, Options::value<std::string>("0x00000000"));
    options.add({  "--thumb,-t", "Disassemble as Thumb"   }, Options::value<bool>());
    options.add({ "--format,-f", "Output format", "value" }, Options::value<std::string>("{addr:08X}  {instr:08X}  {mnemonic}"));
    options.add({       "input", "Input file"             }, Options::value<fs::path>()->positional());
    options.add({      "output", "Output file"            }, Options::value<fs::path>()->positional());

    try
    {
        OptionsResult opts = options.parse(argc, argv);

        if (*opts.find<bool>("--help"))
        {
            fmt::print("{}", options.help());
            std::exit(0);
        }

        fs::path input = *opts.find<fs::path>("input");

        std::vector<u8> data;
        if (fs::read(input, data) != fs::Status::Ok)
            throw FormattedError("Cannot open file {}", input);

        fs::path output = *opts.find<fs::path>("output");

        std::ofstream stream(output);
        if (!stream || !stream.is_open())
            throw FormattedError("Cannot open file {}", output);

        u32 addr = parseBase(*opts.find<std::string>("--base"));

        std::string format = *opts.find<std::string>("--format") + "\n";

        if (*opts.find<bool>("--thumb"))
        {
            u32  lr  = 0;
            u32  len = data.size() / 2;
            u16* rom = reinterpret_cast<u16*>(data.data());

            for (u16 instr : PointerRange(rom, len))
            {
                stream << fmt::format(
                    format,
                    fmt::arg("addr", addr),
                    fmt::arg("instr", instr),
                    fmt::arg("mnemonic", disassemble(instr, addr + 4, lr)));

                uint offset = shell::bit::seq<0, 11>(instr);
                offset = shell::bit::signEx<11>(offset);
                offset <<= 12;
                lr = (addr + 4) + offset;

                addr += 2;
            }
        }
        else
        {
            u32  len = data.size() / 4;
            u32* rom = reinterpret_cast<u32*>(data.data());

            for (u32 instr : PointerRange(rom, len))
            {
                stream << fmt::format(
                    format,
                    fmt::arg("addr", addr),
                    fmt::arg("instr", instr),
                    fmt::arg("mnemonic", disassemble(instr, addr + 8)));

                addr += 4;
            }
        }
    }
    catch (const ParseError& error)
    {
        fmt::print("{}", options.help());
    }
    catch (const std::exception& exception)
    {
        fmt::print("{}\n\n{}", exception.what(), options.help());
    }
    return 0;
}
