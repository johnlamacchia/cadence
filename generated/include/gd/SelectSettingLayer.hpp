#pragma once

class SelectSettingDelegate;
class cocos2d;

class SelectSettingLayer : public FLAlertLayer {
public:
    virtual ~SelectSettingLayer();
    virtual void keyBackClicked();

    static SelectSettingLayer* create(SelectSettingType type, int value);
    static std::string frameForItem(SelectSettingType type, int index);
    static std::string frameForValue(SelectSettingType type, int value);
    static int idxToValue(SelectSettingType type, int idx);
    static int valueToIdx(SelectSettingType type, int value);

    std::string getSelectedFrame();
    int getSelectedValue();
    bool init(SelectSettingType type, int value);
    void onClose(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);

    cocos2d::CCArray* m_settingSprites;
    int m_settingID;
    SelectSettingType m_type;
    SelectSettingDelegate* m_delegate;

};
