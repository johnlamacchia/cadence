#pragma once

class BoomScrollLayer;
class cocos2d;

class EditButtonBar : public cocos2d::CCNode {
public:
    virtual ~EditButtonBar();

    static EditButtonBar* create(cocos2d::CCArray* objects, cocos2d::CCPoint position, int tab, bool hasCreateItems, int columns, int rows);

    int getPage();
    void goToPage(int page);
    bool init(cocos2d::CCArray* objects, cocos2d::CCPoint position, int tab, bool hasCreateItems, int columns, int rows);
    void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage);
    void onDotClicked(cocos2d::CCObject* sender);
    void onLeft(cocos2d::CCObject* sender);
    void onRight(cocos2d::CCObject* sender);
    void reloadItems(int rowCount, int columnCount);
    void /* TodoReturn */ updateDotState();

    cocos2d::CCPoint m_position;
    int m_tabIndex;
    bool m_hasCreateItems;
    cocos2d::CCArray* m_buttonArray;
    BoomScrollLayer* m_scrollLayer;
    cocos2d::CCArray* m_pagesArray;
    cocos2d::CCMenu* m_dotMenu;

};
