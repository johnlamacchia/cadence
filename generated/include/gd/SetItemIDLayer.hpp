#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetItemIDLayer : public SetupTriggerPopup {
public:
    virtual ~SetItemIDLayer();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetItemIDLayer* create(EffectGameObject* object, cocos2d::CCArray* objects);

    SetItemIDLayer();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateEditorLabel();

};
