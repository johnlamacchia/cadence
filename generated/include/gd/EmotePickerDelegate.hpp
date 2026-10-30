#pragma once

class EmoteDef;
class EmotePickerPopup;
class cocos2d;

class EmotePickerDelegate {
public:
    virtual void emotePickerClosed(EmotePickerPopup*);
    virtual void emotePicked(EmotePickerPopup*, EmoteDef const*);
    virtual void emotePickerKeyDown(EmotePickerPopup*, cocos2d::enumKeyCodes, double);

};
