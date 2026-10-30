#pragma once

class ButtonSprite;
class ColorSelectDelegate;
class GJSpecialColorSelectDelegate;
class cocos2d;

class GJSpecialColorSelect : public FLAlertLayer {
public:
    virtual ~GJSpecialColorSelect();
    virtual void keyBackClicked();

    static GJSpecialColorSelect* create(int id, GJSpecialColorSelectDelegate* delegate, ColorSelectType type);
    static const char* textForColorIdx(int index);

    ButtonSprite* getButtonByTag(int tag);
    void highlightSelected(ButtonSprite* sprite);
    bool init(int id, GJSpecialColorSelectDelegate* delegate, ColorSelectType type);
    void onClose(cocos2d::CCObject* sender);
    void onSelectColor(cocos2d::CCObject* sender);

    GJSpecialColorSelectDelegate* m_delegate;
    int m_colorID;
    cocos2d::CCArray* m_buttonSprites;

};
