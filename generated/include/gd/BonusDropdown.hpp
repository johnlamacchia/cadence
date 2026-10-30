#pragma once

class Dropdown;
class cocos2d;

class BonusDropdown : public cocos2d::CCNode {
public:
    static BonusDropdown* create(std::string text, int price);

    bool init(std::string text, int price);
    void show();

    cocos2d::CCLayerColor* m_layerColor;
    float m_yPosition;
    cocos2d::CCScene* m_scene;

};
