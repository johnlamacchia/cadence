#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class CCTextInputNode;
class cocos2d;

class MoreSearchLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static MoreSearchLayer* create();

    MoreSearchLayer();
    ~MoreSearchLayer();
    void audioNext(cocos2d::CCObject* sender);
    void audioPrevious(cocos2d::CCObject* sender);
    CCMenuItemToggler* createToggleButton(std::string label, cocos2d::SEL_MenuHandler callback, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    void onClose(cocos2d::CCObject* sender);
    void onCoins(cocos2d::CCObject* sender);
    void onCompleted(cocos2d::CCObject* sender);
    void onEpic(cocos2d::CCObject* sender);
    void onFeatured(cocos2d::CCObject* sender);
    void onFollowed(cocos2d::CCObject* sender);
    void onFriends(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onLegendary(cocos2d::CCObject* sender);
    void onMythic(cocos2d::CCObject* sender);
    void onNoStar(cocos2d::CCObject* sender);
    void onOriginal(cocos2d::CCObject* sender);
    void onSongFilter(cocos2d::CCObject* sender);
    void onSongMode(cocos2d::CCObject* sender);
    void onTwoPlayer(cocos2d::CCObject* sender);
    void onUncompleted(cocos2d::CCObject* sender);
    void selectSong(int songID);
    void toggleSongNodes(bool custom, bool disable);
    void updateAudioLabel();

    cocos2d::CCLabelBMFont* m_audioTrackName;
    CCMenuItemSpriteExtra* m_songLeftBtn;
    CCMenuItemSpriteExtra* m_songRightBtn;
    CCMenuItemSpriteExtra* m_normalBtn;
    CCMenuItemSpriteExtra* m_customBtn;
    CCTextInputNode* m_enterSongID;
    cocos2d::CCArray* m_commonSongNodes;
    cocos2d::CCArray* m_normalSongNodes;
    cocos2d::CCArray* m_customSongNodes;

};
