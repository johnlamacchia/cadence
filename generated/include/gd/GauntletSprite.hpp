#pragma once

class cocos2d;

class GauntletSprite : public cocos2d::CCNode {
public:
    virtual ~GauntletSprite();

    static GauntletSprite* create(GauntletType type, bool locked);

    GauntletSprite();
    void addLockedSprite();
    void addNormalSprite();
    cocos2d::ccColor3B colorForType(GauntletType type);
    bool init(GauntletType type, bool locked);
    float luminanceForType(GauntletType type);
    void toggleLockedSprite(bool locked);

    GauntletType m_gauntletType;

};
