#pragma once

class CharacterColorDelegate {
public:
    virtual void playerColorChanged();
    virtual void showUnlockPopup(int id, UnlockType type);

};
