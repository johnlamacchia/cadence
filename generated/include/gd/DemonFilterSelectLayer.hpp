#pragma once

class DemonFilterDelegate;
class cocos2d;

class DemonFilterSelectLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static DemonFilterSelectLayer* create();

    DemonFilterSelectLayer();
    ~DemonFilterSelectLayer();
    void onClose(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);

    cocos2d::CCArray* m_demons;
    void* m_unkPtr;
    int m_currentDemon;
    DemonFilterDelegate* m_delegate;

};
