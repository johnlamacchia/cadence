#pragma once

class SFXFolderPickerPopup;

class SFXFolderPickerDelegate {
public:
    virtual void sfxFolderPicked(SFXFolderPickerPopup*, int, std::string const&);
    virtual void sfxFoldersPicked(SFXFolderPickerPopup*, std::set<int> const&);

};
