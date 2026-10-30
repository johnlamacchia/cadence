#pragma once

class CCMenuItemSpriteExtra;

class TabbedPopupTab {
public:
    std::string m_name;
    std::string m_icon;
    int m_groupID;
    int m_page;
    int m_pageCount;
    int m_itemCount;
    CCMenuItemSpriteExtra* m_button;

};
