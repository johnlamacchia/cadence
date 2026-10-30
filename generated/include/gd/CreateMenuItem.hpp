#pragma once

class cocos2d;

class CreateMenuItem : public CCMenuItemSpriteExtra {
public:
    virtual ~CreateMenuItem();

    static CreateMenuItem* create(cocos2d::CCNode* normal, cocos2d::CCNode* selected, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

    bool init(cocos2d::CCNode* normal, cocos2d::CCNode* selected, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

    std::string m_spriteFileName;
    int m_objectID;
    int m_pageIndex;
    int m_tabIndex;

};
