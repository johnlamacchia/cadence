#pragma once

class cocos2d;

class GJChestSprite : public cocos2d::CCSprite {
public:
    virtual ~GJChestSprite();
    virtual void setOpacity(unsigned char opacity);
    virtual void setColor(cocos2d::ccColor3B const& color);

    static GJChestSprite* create(int chestType);

    bool init(int chestType);
    void switchToState(ChestSpriteState state, bool noGlow);

    int m_chestType;
    ChestSpriteState m_spriteState;
    bool m_dark;

};
