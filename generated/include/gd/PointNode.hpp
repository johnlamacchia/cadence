#pragma once

class cocos2d;

class PointNode : public cocos2d::CCObject {
public:
    virtual ~PointNode();

    static PointNode* create(cocos2d::CCPoint point);

    PointNode();
    bool init(cocos2d::CCPoint point);

    cocos2d::CCPoint m_point;

};
