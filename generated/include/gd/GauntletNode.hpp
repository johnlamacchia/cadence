#pragma once

class GJMapPack;
class cocos2d;

class GauntletNode : public cocos2d::CCNode {
public:
    virtual ~GauntletNode();

    static GauntletNode* create(GJMapPack* gauntlet);
    static std::string frameForType(GauntletType type);
    static std::string nameForType(GauntletType type);

    GauntletNode();
    void generateNode();
    bool init(GJMapPack* gauntlet);
    void onClaimReward();
    void onUnlock();
    void showUnlockAnimation();

    cocos2d::CCNode* m_gauntletInfoNode;
    cocos2d::CCNode* m_rewardNode;
    cocos2d::ccColor3B m_labelColor;
    cocos2d::ccColor3B m_backgroundColor;
    GJMapPack* m_gauntlet;
    bool m_locked;

};
