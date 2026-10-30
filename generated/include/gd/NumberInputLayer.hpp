#pragma once

class CCMenuItemSpriteExtra;
class NumberInputDelegate;
class cocos2d;

class NumberInputLayer : public FLAlertLayer {
public:
    virtual ~NumberInputLayer();
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void onDone(cocos2d::CCObject* sender);

    static NumberInputLayer* create();

    NumberInputLayer();
    void deleteLast();
    void inputNumber(int num);
    void onClose(cocos2d::CCObject* sender);
    void onNumber(cocos2d::CCObject* sender);
    void updateNumberState();

    cocos2d::CCLabelBMFont* m_inputLabel;
    cocos2d::CCLabelBMFont* m_title;
    CCMenuItemSpriteExtra* m_okButton;
    int m_minimum;
    int m_maximum;
    std::string m_prefixString;
    std::string m_inputString;
    NumberInputDelegate* m_delegate;

};
