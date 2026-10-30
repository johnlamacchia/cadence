#pragma once

class CCTextInputNode;
class EditorUI;
class cocos2d;

class MoveStepPopup : public EditorWindowPopup {
public:
    virtual ~MoveStepPopup();
    virtual void textChanged(CCTextInputNode*);
    virtual void valueDidChange(int, float);
    virtual float getValue(int);
    virtual void onBuildWindowContent();

    static MoveStepPopup* create(EditorUI*);

    bool init(EditorUI*);
    void onReset(cocos2d::CCObject* sender);

};
