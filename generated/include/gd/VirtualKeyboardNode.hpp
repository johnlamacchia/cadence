#pragma once

class InputCaptureDelegate;
class cocos2d;

class VirtualKeyboardNode : public cocos2d::CCNode {
public:
    static VirtualKeyboardNode* create(InputCaptureDelegate*);

    void /* TodoReturn */ buildKeys();
    bool init(InputCaptureDelegate*);
    void onKey(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshModifierVisuals();
    void /* TodoReturn */ resetModifiers();

};
