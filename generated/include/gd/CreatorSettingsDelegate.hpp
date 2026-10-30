#pragma once

class CreatorSettingsDelegate {
public:
    virtual ~CreatorSettingsDelegate();
    virtual void creatorDescriptionChanged(std::string const&);

};
