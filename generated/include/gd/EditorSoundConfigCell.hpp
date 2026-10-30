#pragma once

class GJEditorSoundConfig;
class cocos2d;

class EditorSoundConfigCell : public TableViewCell {
public:
    virtual ~EditorSoundConfigCell();
    virtual bool init();

    static EditorSoundConfigCell* create(float, float);

    EditorSoundConfigCell(char const*, float, float);
    void /* TodoReturn */ loadFromConfig(GJEditorSoundConfig*);
    void onInfoPopup(cocos2d::CCObject* sender);
    void onView(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateBGColor(int);

};
