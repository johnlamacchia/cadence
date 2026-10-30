#pragma once

class BoomListView;
class GJComment;
class cocos2d;

class GJCommentListLayer : public cocos2d::CCLayerColor {
public:
    virtual ~GJCommentListLayer();

    static GJCommentListLayer* create(BoomListView* listView, char const* title, cocos2d::ccColor4B color, float width, float height, bool blueBorder);

    bool init(BoomListView* listView, char const* title, cocos2d::ccColor4B color, float width, float height, bool blueBorder);

    BoomListView* m_list;

};
