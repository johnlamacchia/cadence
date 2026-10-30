#pragma once

class cocos2d;

class LevelFeatureLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static LevelFeatureLayer* create(int levelID);

    bool init(int levelID);
    void onClose(cocos2d::CCObject* sender);
    void onDown(cocos2d::CCObject* sender);
    void onDown2(cocos2d::CCObject* sender);
    void onRemoveValues(cocos2d::CCObject* sender);
    void onSetEpicOnly(cocos2d::CCObject* sender);
    void onSetFeatured(cocos2d::CCObject* sender);
    void onToggleEpic(cocos2d::CCObject* sender);
    void onUp(cocos2d::CCObject* sender);
    void onUp2(cocos2d::CCObject* sender);
    void updateStars();

    cocos2d::CCLabelBMFont* m_featureLabel;
    int m_levelID;
    int m_featurePosition;
    bool m_epicOnly;
    cocos2d::CCSprite* m_epicSprite;

};
