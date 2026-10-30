#pragma once

class CCTextInputNode;
class SetIDPopupDelegate;
class cocos2d;

class SetIDPopup : public FLAlertLayer {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void valueChanged();

    static SetIDPopup* create(int current, int begin, int end, std::string title, std::string button, bool resetButton, int defaultValue, float offset, bool numberInput, bool arrows);

    SetIDPopup();
    ~SetIDPopup();
    bool init(int current, int begin, int end, std::string title, std::string button, bool resetButton, int defaultValue, float offset, bool numberInput, bool arrows);
    void onCancel(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onItemIDArrow(cocos2d::CCObject* sender);
    void onResetValue(cocos2d::CCObject* sender);
    void updateTextInputLabel();

    CCTextInputNode* m_inputNode;
    int m_value;
    bool m_disableDelegate;
    bool m_cancelled;
    int m_minimum;
    int m_maximum;
    int m_default;
    SetIDPopupDelegate* m_delegate;

};
