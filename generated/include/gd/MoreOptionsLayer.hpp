#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GJDropDownLayer;
class OptionsLayer;
class cocos2d;

class MoreOptionsLayer : public FLAlertLayer, public GooglePlayDelegate, public GJDropDownLayerDelegate {
public:
    virtual bool init();
    virtual void keyBackClicked();
    virtual void googlePlaySignedIn();
    virtual void dropDownLayerWillClose(GJDropDownLayer* layer);

    static MoreOptionsLayer* create();

    MoreOptionsLayer();
    ~MoreOptionsLayer();
    void addToggle(char const* label, char const* key, char const* description);
    int countForPage(int page);
    void goToPage(int page);
    void incrementCountForPage(int page);
    const char* infoKey(int index);
    cocos2d::CCLayer* layerForPage(int page);
    const char* layerKey(int page);
    cocos2d::CCPoint nextPosition(int page);
    const char* objectKey(int page);
    cocos2d::CCArray* objectsForPage(int page);
    void offsetToNextPage();
    void onClose(cocos2d::CCObject* sender);
    void onFMODDebug(cocos2d::CCObject* sender);
    void onGPSignIn(cocos2d::CCObject* sender);
    void onGPSignOut(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onKeybindings(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onParental(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onSongBrowser(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    const char* pageKey(int page);
    void toggleGP();

    int m_page;
    int m_toggleCount;
    int m_pageCount;
    cocos2d::CCDictionary* m_variables;
    cocos2d::CCDictionary* m_objects;
    CCMenuItemSpriteExtra* m_leftBtn;
    CCMenuItemSpriteExtra* m_rightBtn;
    CCTextInputNode* m_offsetInput;
    CCMenuItemSpriteExtra* m_gpSignInBtn;
    CCMenuItemSpriteExtra* m_gpSignOutBtn;
    cocos2d::CCLabelBMFont* m_categoryLabel;

};
