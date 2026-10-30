#pragma once

class cocos2d;

class GJActionManager : public cocos2d::CCNode {
public:
    virtual bool init();

    static GJActionManager* create();

    GJActionManager();
    ~GJActionManager();
    cocos2d::CCAction* getInternalAction(int id);
    void runInternalAction(cocos2d::CCAction* action, cocos2d::CCNode* target);
    void stopAllInternalActions();
    void stopInternalAction(int id);
    void updateInternalActions(float dt, bool remove);

    cocos2d::CCDictionary* m_internalActions;

};
