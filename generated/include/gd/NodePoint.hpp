#pragma once

class cocos2d;

class NodePoint : public cocos2d::CCObject {
public:
    static NodePoint* create(cocos2d::CCPoint point);

    NodePoint();
    bool init(cocos2d::CCPoint point);

    cocos2d::CCPoint m_point;

};
