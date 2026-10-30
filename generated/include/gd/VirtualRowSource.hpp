#pragma once

class cocos2d;

class VirtualRowSource {
public:
    virtual int virtualRowCount();
    virtual int virtualRowHeight(int);
    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*);
    virtual void unmountVirtualRow(int, cocos2d::CCNode*);

};
