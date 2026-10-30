#pragma once

class InputCaptureDelegate;
class cocos2d;

class ControllerInputNode : public cocos2d::CCNode {
public:
    virtual ~ControllerInputNode();

    static ControllerInputNode* create(InputCaptureDelegate*, cocos2d::CCSize const&);

    bool init(InputCaptureDelegate*, cocos2d::CCSize const&);
    void onButton(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshVisuals();
    void setActiveKey(int);
    void setController2(bool);

};
