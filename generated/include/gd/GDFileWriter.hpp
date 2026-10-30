#pragma once

class GDFileWriter {
public:
    static bool writeAtomic(std::string const& path, std::string const& data);

};
