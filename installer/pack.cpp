// Packs the files the installer carries into one blob, which installer.rc embeds as a resource.
//
//   cadence_pack <out.bin> <list.txt>
//
// Each line of the list is "<source file>|<path inside the install folder>", e.g.
//   C:/build/loader/Release/winmm.dll|winmm.dll
//   C:/build/mods/hitboxes/Release/hitboxes.dll|mods/hitboxes.dll
//
// Blob layout (little endian): "CDNPAK01", u32 count, then per file:
//   u32 path length, path bytes (forward slashes), u64 size, file bytes.
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

template <class T>
void put(std::ofstream& out, T value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::fprintf(stderr, "usage: cadence_pack <out.bin> <list.txt>\n");
        return 2;
    }

    struct Entry { std::string source, dest; };
    std::vector<Entry> entries;
    std::ifstream list(argv[2]);
    for (std::string line; std::getline(list, line);) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        if (line.empty()) continue;
        auto bar = line.rfind('|');
        if (bar == std::string::npos) {
            std::fprintf(stderr, "bad line in %s: %s\n", argv[2], line.c_str());
            return 2;
        }
        entries.push_back({line.substr(0, bar), line.substr(bar + 1)});
    }
    if (entries.empty()) {
        std::fprintf(stderr, "%s lists no files\n", argv[2]);
        return 2;
    }

    std::ofstream out(argv[1], std::ios::binary | std::ios::trunc);
    out.write("CDNPAK01", 8);
    put<uint32_t>(out, static_cast<uint32_t>(entries.size()));
    for (const Entry& e : entries) {
        std::ifstream in(e.source, std::ios::binary);
        if (!in) {
            std::fprintf(stderr, "cannot read %s\n", e.source.c_str());
            return 1;
        }
        std::vector<char> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        put<uint32_t>(out, static_cast<uint32_t>(e.dest.size()));
        out.write(e.dest.data(), static_cast<std::streamsize>(e.dest.size()));
        put<uint64_t>(out, static_cast<uint64_t>(bytes.size()));
        out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    }
    out.flush();
    if (!out) {
        std::fprintf(stderr, "could not write %s\n", argv[1]);
        return 1;
    }
    std::printf("packed %zu files into %s\n", entries.size(), argv[1]);
    return 0;
}
