#pragma once

class CCTextInputNode;
class CameraTriggerGameObject;
class GameObject;
class cocos2d;

class SetupCameraEdgePopup : public SetupTriggerPopup {
public:
    virtual ~SetupCameraEdgePopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupCameraEdgePopup* create(CameraTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(CameraTriggerGameObject* object, cocos2d::CCArray* objects);
    void onCameraEdge(cocos2d::CCObject* sender);
    void onUnlockEdge(cocos2d::CCObject* sender);
    void updateTargetID();
    void updateTextInputLabel();

    CCTextInputNode* m_targetIDInput;
    int m_targetID;
    int m_edgeDirection;
    cocos2d::CCArray* m_directionToggles;

};
