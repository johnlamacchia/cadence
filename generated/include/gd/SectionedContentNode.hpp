#pragma once

class cocos2d;

class SectionedContentNode : public cocos2d::CCNode {
public:
    virtual ~SectionedContentNode();
    virtual void addChild(cocos2d::CCNode*, int, int);
    virtual void removeAllChildrenWithCleanup(bool);

};
