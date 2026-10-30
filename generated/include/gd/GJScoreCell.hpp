#pragma once

class FLAlertLayer;
class GJUserScore;
class cocos2d;

class GJScoreCell : public TableViewCell, public FLAlertLayerProtocol {
public:
    virtual ~GJScoreCell();
    virtual bool init();
    virtual void draw();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    GJScoreCell(char const* identifier, float width, float height);
    void loadFromScore(GJUserScore* score);
    void onBan(cocos2d::CCObject* sender);
    void onCheck(cocos2d::CCObject* sender);
    void onMoreLevels(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    GJUserScore* m_score;

};
