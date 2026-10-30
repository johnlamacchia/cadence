#pragma once

class cocos2d;

class NewsPopup : public FLAlertLayer, public FLAlertLayerProtocol, public MusicDownloadDelegate {
public:
    virtual ~NewsPopup();
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void musicActionFinished(GJMusicAction);
    virtual void musicActionFailed(GJMusicAction);
    virtual void /* TodoReturn */ newsImageDownloaded(std::string);
    virtual void /* TodoReturn */ newsImageFailed(std::string);

    static NewsPopup* create();

    void onBlockClicked(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void /* TodoReturn */ pageReferencesImage(int, std::string);
    void /* TodoReturn */ prefetchPageImages(int);
    void /* TodoReturn */ rebuildPage(int);
    void /* TodoReturn */ refreshFromManager();

};
