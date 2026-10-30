#pragma once

class cocos2d;

class CurrencySprite : public CCSpritePlus {
public:
    static CurrencySprite* create(CurrencySpriteType type, bool burst);
    static CurrencySprite* createWithSprite(cocos2d::CCSprite* sprite);
    static CurrencySpriteType rewardToSpriteType(int rewardType);
    static std::string spriteTypeToStat(CurrencySpriteType type);

    CurrencySprite();
    ~CurrencySprite();
    bool init(CurrencySpriteType type, bool burst);
    bool initWithSprite(cocos2d::CCSprite* sprite);

    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    float m_remaining;
    int m_count;
    cocos2d::CCParticleSystemQuad* m_particleSystem;
    CurrencySpriteType m_spriteType;
    cocos2d::CCPoint m_position;
    cocos2d::CCSprite* m_burstSprite;

};
