#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupSpawnPopup : public SetupTriggerPopup {
public:
    virtual ~SetupSpawnPopup();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupSpawnPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    void addRemap(int oldID, int newID);
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onAddRemap(cocos2d::CCObject* sender);
    void onDeleteRemap(cocos2d::CCObject* sender);
    void onPasteRemap(cocos2d::CCObject* sender);
    void onSelectRemap(cocos2d::CCObject* sender);
    void queueUpdateButtons();
    void updateRemapButtons(float dt);

    bool m_isBusy;
    cocos2d::CCArray* m_remapButtons;
    std::vector<int> m_remapGroups;
    int m_remapOriginalID;
    int m_remapNewID;

};
