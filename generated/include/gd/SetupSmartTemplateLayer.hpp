#pragma once

class CCTextInputNode;
class GJSmartTemplate;
class cocos2d;

class SetupSmartTemplateLayer : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~SetupSmartTemplateLayer();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static SetupSmartTemplateLayer* create(GJSmartTemplate* smartTemplate);

    bool init(GJSmartTemplate* smartTemplate);
    void onBack(cocos2d::CCObject* sender);
    void onClick(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);

    GJSmartTemplate* m_smartTemplate;
    CCTextInputNode* m_nameInput;
    cocos2d::CCNode* m_unk298;

};
