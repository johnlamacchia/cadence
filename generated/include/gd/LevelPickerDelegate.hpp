#pragma once

class GJGameLevel;
class GJLevelList;

class LevelPickerDelegate {
public:
    virtual void levelPicked(GJGameLevel*);
    virtual void listPicked(GJLevelList*);
    virtual void shareIDPicked(int, bool);

};
