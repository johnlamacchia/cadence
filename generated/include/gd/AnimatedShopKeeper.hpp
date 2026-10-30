#pragma once

class AnimatedShopKeeper : public CCAnimatedSprite {
public:
    virtual ~AnimatedShopKeeper();
    virtual void animationFinished(char const* key);

    static AnimatedShopKeeper* create(ShopType type);

    AnimatedShopKeeper();
    bool init(ShopType type);
    void playReactAnimation();
    void startAnimating();

    ShopType m_type;
    int m_idleInt1;
    int m_idleInt2;
    bool m_looking;
    bool m_reacting;
    int m_reactCount;
    int m_gruntIndex;

};
