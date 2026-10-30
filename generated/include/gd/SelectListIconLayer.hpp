#pragma once

class SelectListIconDelegate;
class cocos2d;

class SelectListIconLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static SelectListIconLayer* create(int difficulty);

    SelectListIconLayer();
    ~SelectListIconLayer();
    bool init(int difficulty);
    void onClose(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);

    cocos2d::CCArray* m_difficulties;
    int m_currentDifficulty;
    SelectListIconDelegate* m_delegate;

};
