#pragma once

class CCNodeContainer : public cocos2d::CCNode {
public:
    virtual ~CCNodeContainer();
    virtual bool init();
    virtual void visit();

    static CCNodeContainer* create();

};
