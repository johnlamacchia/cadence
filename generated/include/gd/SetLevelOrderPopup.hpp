#pragma once

class cocos2d;

class SetLevelOrderPopup : public SetIDPopup {
public:
    static SetLevelOrderPopup* create(int levelID, int order, int amount);

    SetLevelOrderPopup();
    bool init(int levelID, int order, int amount);
    void onOrderButton(cocos2d::CCObject* sender);

    int m_levelID;

};
