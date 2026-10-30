#pragma once

class BoomScrollLayer;
class ListButtonBarDelegate;
class cocos2d;

class ListButtonBar : public cocos2d::CCNode {
public:
    virtual ~ListButtonBar();

    static ListButtonBar* create(cocos2d::CCArray* items, cocos2d::CCPoint position, int columns, int rows, float columnOffset, float rowOffset, float offset, float arrowOffset, int arrowType);

    int getPage();
    void goToPage(int page);
    bool init(cocos2d::CCArray* items, cocos2d::CCPoint position, int columns, int rows, float columnOffset, float rowOffset, float offset, float arrowOffset, int arrowType);
    void onLeft(cocos2d::CCObject* sender);
    void onRight(cocos2d::CCObject* sender);

    BoomScrollLayer* m_scrollLayer;
    cocos2d::CCArray* m_pages;
    ListButtonBarDelegate* m_delegate;
    bool m_useMoveAnimation;

};
