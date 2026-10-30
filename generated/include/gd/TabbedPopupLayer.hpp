#pragma once

class CCMenuItemSpriteExtra;
class TabbedPopupTab;
class cocos2d;

class TabbedPopupLayer : public SetupTriggerPopup {
public:
    virtual ~TabbedPopupLayer();
    virtual void pageChanged();
    virtual void showTab(int tab);

    void addCloseButton();
    int addTab(std::string const& name, char const* icon);
    void beginTab(int tab);
    void createTabButtons();
    void createTabButtons(float, float, float, float, float, float);
    int ensurePopupPage(int tab, int page);
    int getPopupPage(int tab, int page);
    void offsetToNextPage(int perPage);
    void onTab(cocos2d::CCObject* sender);
    void onTabPage(cocos2d::CCObject* sender);
    int pageForButtonCount(int count, int perPage);
    void refreshPageButtons();
    void refreshTabButtons(char const* selectedBG, char const* normalBG);

    std::vector<TabbedPopupTab> m_tabs;
    std::vector<std::vector<int>> m_tabPages;
    int m_currentTab;
    int m_buildTab;
    CCMenuItemSpriteExtra* m_prevPageButton;
    CCMenuItemSpriteExtra* m_nextPageButton;

};
