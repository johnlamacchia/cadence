#pragma once

class GJUnlockableItem : public cocos2d::CCObject {
public:
    virtual ~GJUnlockableItem();

    static GJUnlockableItem* create();

    GJUnlockableItem();
    bool init();

};
