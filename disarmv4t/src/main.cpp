#include <shell/bit.h>
#include <shell/constants.h>
#include <shell/filesystem.h>
#include <shell/fmt.h>
#include <shell/int.h>
#include <shell/main.h>
#include <shell/options.h>

#include "disassemble.h"

namespace fs = shell::filesystem;

int main(int argc, char* argv[])
{
    using namespace shell;

    Options options("disarmv4t");
    options.add({   "-b,--base", "Base address", "value"  }, Options::value<u32>(0));
    options.add({  "-t,--thumb", "Disassemble as Thumb"   }, Options::value<bool>(false));
    options.add({ "-f,--format", "Output format", "value" }, Options::value<std::string>("{addr:08X}  {instr:08X}  {mnemonic}"));
    options.add({       "input", "Input file"             }, Options::value<fs::path>()->positional());
    options.add({      "output", "Output file"            }, Options::value<fs::path>()->positional());

    try
    {
        OptionsResult result = options.parse(argc, argv);

        auto addr   = *result.find<u32>("--base");
        auto size   = *result.find<bool>("--thumb") ? 2 : 4;
        auto format = *result.find<std::string>("--format");
        auto input  = *result.find<fs::path>("input");
        auto output = *result.find<fs::path>("output");

        auto [status, data] = fs::read<std::vector<u8>>(input);

        if (status != fs::Status::Ok)
        {
            fmt::print("Cannot read file {}", input);
            return 1;
        }

        data.resize(data.size() + data.size() % size, 0);

        std::ofstream stream(output, std::ios::binary);
        if (!stream || !stream.is_open())
        {
            fmt::print("Cannot open file {}", output);
            return 2;
        }

        if (size == 4)
        {
            for (u32 instr : PointerRange(reinterpret_cast<u32*>(data.data()), data.size() / 4))
            {
                stream << fmt::format(
                    format,
                    fmt::arg("addr", addr),
                    fmt::arg("instr", instr),
                    fmt::arg("mnemonic", disassemble(instr, addr + 8)));

                stream << kLineBreak;

                addr += 4;
            }
        }
        else
        {
            u32 lr = 0;

            for (u16 instr : PointerRange(reinterpret_cast<u16*>(data.data()), data.size() / 2))
            {
                stream << fmt::format(
                    format,
                    fmt::arg("addr", addr),
                    fmt::arg("instr", instr),
                    fmt::arg("mnemonic", disassemble(instr, addr + 4, lr)));

                stream << kLineBreak;

                uint offset = shell::bit::seq<0, 11>(instr);
                offset = shell::bit::signEx<11>(offset);
                offset <<= 12;
                lr = addr + 4 + offset;

                addr += 2;
            }
        }
        return 0;
    }
    catch (const std::exception& ex)
    {
        fmt::print("{}\n\n{}", ex.what(), options.help());
        return 3;
    }
}
