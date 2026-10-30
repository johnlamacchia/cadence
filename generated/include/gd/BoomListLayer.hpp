#pragma once

class BoomListView;

class BoomListLayer : public cocos2d::CCLayerColor {
public:
    static BoomListLayer* create(BoomListView* listView, char const* title);

    bool init(BoomListView* listView, char const* title);

};
