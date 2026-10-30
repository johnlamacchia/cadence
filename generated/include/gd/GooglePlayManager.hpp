#pragma once

class GooglePlayDelegate;

class GooglePlayManager : public cocos2d::CCNode {
public:
    virtual ~GooglePlayManager();
    virtual bool init();

    static GooglePlayManager* sharedState();

    void googlePlaySignedIn();

    GooglePlayDelegate* m_delegate1;
    GooglePlayDelegate* m_delegate2;

};
