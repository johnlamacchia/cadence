#pragma once

class BoomListView;
class cocos2d;

class GJListLayer : public cocos2d::CCLayerColor {
public:
    virtual ~GJListLayer();

    static GJListLayer* create(BoomListView* listView, char const* title, cocos2d::ccColor4B color, float width, float height, int type);

    bool init(BoomListView* listView, char const* title, cocos2d::ccColor4B color, float width, float height, int type);

    BoomListView* m_listView;

};
