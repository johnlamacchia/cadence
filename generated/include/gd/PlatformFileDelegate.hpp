#pragma once

class PlatformFileDelegate {
public:
    virtual void filesPicked(std::vector<std::string> const&);
    virtual void fileShareFinished(bool);

};
