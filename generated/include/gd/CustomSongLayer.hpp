#pragma once

class CCTextInputNode;
class CustomSongDelegate;
class CustomSongLayerDelegate;
class CustomSongWidget;
class GJDropDownLayer;
class MusicBrowser;
class cocos2d;

class CustomSongLayer : public FLAlertLayer, public GJDropDownLayerDelegate, public MusicBrowserDelegate {
public:
    virtual ~CustomSongLayer();
    virtual void keyBackClicked();
    virtual void show();
    virtual void textChanged(CCTextInputNode* node);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void dropDownLayerWillClose(GJDropDownLayer* layer);
    virtual void musicBrowserClosed(MusicBrowser* browser);

    static CustomSongLayer* create(CustomSongDelegate* delegate);

    CustomSongLayer();
    bool init(CustomSongDelegate* delegate);
    void onClose(cocos2d::CCObject* sender);
    void onCreateLines(cocos2d::CCObject* sender);
    void onMusicBrowser(cocos2d::CCObject* sender);
    void onNCSBrowser(cocos2d::CCObject* sender);
    void onNewgroundsBrowser(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onSearch(cocos2d::CCObject* sender);
    void onSongBrowser(cocos2d::CCObject* sender);
    void showNewgroundsMessage();

    CustomSongDelegate* m_songDelegate;
    CCTextInputNode* m_songIDInput;
    CustomSongWidget* m_songWidget;
    CustomSongLayerDelegate* m_delegate;

};
