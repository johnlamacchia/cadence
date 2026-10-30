#pragma once

class ChallengeNode;
class CurrencyRewardLayer;
class GJChallengeItem;
class LoadingCircle;
class cocos2d;

class ChallengesPage : public FLAlertLayer, public FLAlertLayerProtocol, public GJChallengeDelegate, public CurrencyRewardDelegate {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void challengeStatusFinished();
    virtual void challengeStatusFailed();
    virtual void currencyWillExit(CurrencyRewardLayer* layer);

    static ChallengesPage* create();

    ChallengesPage();
    ~ChallengesPage();
    void /* TodoReturn */ canCollectQuests();
    void claimItem(ChallengeNode* node, GJChallengeItem* item, cocos2d::CCPoint position);
    ChallengeNode* createChallengeNode(int number, bool skipAnimation, float animLength, bool isNew);
    void exitNodeAtSlot(int position, float delay);
    void onClose(cocos2d::CCObject* sender);
    void tryGetChallenges();
    void updateDots();
    void updateTimers(float dt);

    cocos2d::CCArray* m_dots;
    cocos2d::CCLabelBMFont* m_countdownLabel;
    LoadingCircle* m_circle;
    bool m_triedToLoad;
    bool m_unkBool;
    CurrencyRewardLayer* m_currencyRewardLayer;
    cocos2d::CCDictionary* m_challengeNodes;

};
