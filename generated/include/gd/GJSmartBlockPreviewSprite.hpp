#pragma once

class GJSmartBlockPreview;

class GJSmartBlockPreviewSprite : public cocos2d::CCNode {
public:
    virtual ~GJSmartBlockPreviewSprite();
    virtual bool init();
    virtual void visit();

    static GJSmartBlockPreviewSprite* create();

    GJSmartBlockPreviewSprite();

};
