#pragma once

class cocos2d;

class SearchButton : public cocos2d::CCSprite {
public:
    virtual ~SearchButton();

    static SearchButton* create(char const*, char const*, float, char const*, float);

    bool init(char const*, char const*, float, char const*, float);

    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCSprite* m_icon;

};
