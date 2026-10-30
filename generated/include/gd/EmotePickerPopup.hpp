#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class EmoteDef;
class EmotePickerDelegate;
class cocos2d;

class EmotePickerPopup : public EditorWindowPopup {
public:
    virtual ~EmotePickerPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void show();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode*);
    virtual void onBuildWindowContent();
    virtual void onWindowResized();

    static EmotePickerPopup* create(CCTextInputNode*, cocos2d::CCPoint, int, int);
    static EmotePickerPopup* toggle(EmotePickerPopup*, EmotePickerDelegate*, cocos2d::CCNode*, CCTextInputNode*, int, int);

    void /* TodoReturn */ addCell(EmoteDef const*, int);
    void /* TodoReturn */ buttonAnchor(cocos2d::CCNode*);
    void /* TodoReturn */ canAddEmote();
    void /* TodoReturn */ createPicker(cocos2d::CCPoint);
    bool init(CCTextInputNode*, cocos2d::CCPoint, int, int);
    void layoutEmoteGrid();
    void onPickEmote(cocos2d::CCObject* sender);
    void /* TodoReturn */ openAt(EmotePickerPopup*, EmotePickerDelegate*, cocos2d::CCNode*);
    void /* TodoReturn */ refreshEnabled();
    void /* TodoReturn */ togglePicker(EmotePickerPopup*, EmotePickerDelegate*, cocos2d::CCNode*);

    CCTextInputNode* m_input;
    int m_maxEmotes;
    int m_maxLength;
    EmotePickerDelegate* m_delegate;
    cocos2d::CCRect m_openPickerButtonRect;
    int m_canAdd;
    std::vector<CCMenuItemSpriteExtra*> m_recentEmoteButtons;

};
