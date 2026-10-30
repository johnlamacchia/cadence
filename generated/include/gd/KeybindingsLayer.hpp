#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GameSettingsLayer;
class KeybindSet;
class KeybindSettingsEntry;
class KeybindSource;
class ScrollableContentLayer;
class cocos2d;

class KeybindingsLayer : public TabbedPopupLayer {
public:
    virtual ~KeybindingsLayer();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void rebuildContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);
    virtual void showTab(int tab);
    virtual void setupTabs();
    virtual std::vector<int> actionsToShow(int tab);
    virtual std::string nameForAction(int action);
    virtual void openBindingEditor(int action, int chord);
    virtual bool showControllerChords();
    virtual std::vector<KeybindSettingsEntry> settingsEntries();
    virtual bool crossCheckGlobalConflicts();

    static KeybindingsLayer* create(GameSettingsLayer* settings);
    static int const* getActionTable(int& count);
    static cocos2d::CCSprite* buildControllerGlyph(int key, bool& isPlayer2);
    static void tintGlyph(cocos2d::CCSprite* sprite, cocos2d::ccColor3B color);

    void buildContentForTab(int tab);
    bool init(GameSettingsLayer* settings);
    void onAddBinding(cocos2d::CCObject* sender);
    void onClearSearch(cocos2d::CCObject* sender);
    void onEditBinding(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);

    GameSettingsLayer* m_settingsLayer;
    KeybindSource* m_keybindSource;
    bool m_ownsKeybindSource;
    CCTextInputNode* m_searchInput;
    cocos2d::extension::CCScale9Sprite* m_searchBG;
    CCMenuItemSpriteExtra* m_clearSearchButton;
    ScrollableContentLayer* m_scrollLayer;
    cocos2d::CCMenu* m_contentMenu;
    bool m_contentMenuTouched;
    std::string m_searchQuery;
    int m_searchTab;

};
