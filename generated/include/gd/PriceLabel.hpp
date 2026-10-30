#pragma once

class cocos2d;

class PriceLabel : public cocos2d::CCNode {
public:
    static PriceLabel* create(int price);

    bool init(int price);
    void setColor(cocos2d::ccColor3B color);
    void setOpacity(float opacity);
    void setPrice(int price);

    cocos2d::CCLabelBMFont* m_priceLabel;
    cocos2d::CCSprite* m_coinSprite;

};
