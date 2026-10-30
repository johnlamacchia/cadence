#pragma once

class ChallengesPage;
class GJChallengeItem;
class cocos2d;

class ChallengeNode : public cocos2d::CCNode {
public:
    static ChallengeNode* create(GJChallengeItem* challengeItem, ChallengesPage* challengesPage, bool isNew);

    ChallengeNode();
    ~ChallengeNode();
    bool init(GJChallengeItem* challengeItem, ChallengesPage* challengesPage, bool isNew);
    void onClaimReward(cocos2d::CCObject* sender);
    void updateTimeLabel(std::string text);

    ChallengesPage* m_challengesPage;
    GJChallengeItem* m_challengeItem;
    cocos2d::CCPoint m_unkPoint;
    cocos2d::CCLabelBMFont* m_countdownLabel;
    bool m_unloaded;

};
