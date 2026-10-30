#pragma once

class SelectPremadeDelegate;
class cocos2d;

class SelectPremadeLayer : public FLAlertLayer {
public:
    virtual ~SelectPremadeLayer();
    virtual bool init();
    virtual void keyBackClicked();

    static SelectPremadeLayer* create();

    SelectPremadeLayer();
    void onClose(cocos2d::CCObject* sender);
    void onSelectPremade(cocos2d::CCObject* sender);

    SelectPremadeDelegate* m_delegate;

};
