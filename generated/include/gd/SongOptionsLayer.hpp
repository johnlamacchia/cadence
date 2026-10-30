#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class CustomSongDelegate;
class OptionsLayer;
class cocos2d;

class SongOptionsLayer : public FLAlertLayer {
public:
    virtual ~SongOptionsLayer();
    virtual void keyBackClicked();

    static SongOptionsLayer* create(CustomSongDelegate* delegate);

    SongOptionsLayer();
    bool init(CustomSongDelegate* delegate);
    void onClose(cocos2d::CCObject* sender);
    void onFadeIn(cocos2d::CCObject* sender);
    void onFadeOut(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onPlayback(cocos2d::CCObject* sender);
    void onSongPersistent(cocos2d::CCObject* sender);
    void updatePlaybackBtn();

    CustomSongDelegate* m_delegate;
    CCTextInputNode* m_offsetInput;
    CCMenuItemSpriteExtra* m_playbackButton;

};
