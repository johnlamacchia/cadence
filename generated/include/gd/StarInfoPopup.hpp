#pragma once

class cocos2d;

class StarInfoPopup : public FLAlertLayer {
public:
    virtual ~StarInfoPopup();
    virtual void keyBackClicked();

    static StarInfoPopup* create(int autos, int easies, int normals, int hards, int harders, int insanes, int dailies, int gauntlets, int maps, bool platformer);
    static StarInfoPopup* createFromString(std::string str);
    static StarInfoPopup* createFromStringMoons(std::string str);

    bool init(int autos, int easies, int normals, int hards, int harders, int insanes, int dailies, int gauntlets, int maps, bool platformer);
    void onClose(cocos2d::CCObject* sender);

};
