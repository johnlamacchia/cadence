#pragma once

class GJBigSprite;
class cocos2d;

class GJBigSpriteNode : public cocos2d::CCNode {
public:
    virtual bool init();

    static GJBigSpriteNode* create();

    std::string m_spriteName;
    cocos2d::CCSprite* m_sprite;
    cocos2d::CCRect m_spriteRect;
    bool m_spriteVisible;

};
