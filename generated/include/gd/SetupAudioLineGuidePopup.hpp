#pragma once

class AudioLineGuideGameObject;
class GameObject;
class SelectSettingLayer;
class cocos2d;

class SetupAudioLineGuidePopup : public SetupTriggerPopup, public SelectSettingDelegate {
public:
    virtual ~SetupAudioLineGuidePopup();
    virtual void selectSettingClosed(SelectSettingLayer* layer);

    static SetupAudioLineGuidePopup* create(AudioLineGuideGameObject* object, cocos2d::CCArray* objects);

    SetupAudioLineGuidePopup();
    bool init(AudioLineGuideGameObject* object, cocos2d::CCArray* objects);
    void onSpeed(cocos2d::CCObject* sender);

    cocos2d::CCSprite* m_speedSprite;

};
