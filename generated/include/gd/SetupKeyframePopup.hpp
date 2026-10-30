#pragma once

class GameObject;
class KeyframeGameObject;
class LevelEditorLayer;
class cocos2d;

class SetupKeyframePopup : public SetupTriggerPopup {
public:
    virtual ~SetupKeyframePopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomButton(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupKeyframePopup* create(KeyframeGameObject* object, cocos2d::CCArray* objects, LevelEditorLayer* layer);

    bool init(KeyframeGameObject* object, cocos2d::CCArray* objects, LevelEditorLayer* layer);
    void onTimeMode(cocos2d::CCObject* sender);
    void refreshPreviewArt();
    void updateTimeModeButtons();

    cocos2d::CCArray* m_timeModeToggles;
    LevelEditorLayer* m_editorLayer;
    bool m_groupIDChanged;

};
