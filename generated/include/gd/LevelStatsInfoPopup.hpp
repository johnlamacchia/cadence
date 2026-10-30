#pragma once

class GJGameLevel;
class cocos2d;

class LevelStatsInfoPopup : public FLAlertLayer {
public:
    virtual ~LevelStatsInfoPopup();
    virtual void keyBackClicked();

    static LevelStatsInfoPopup* create(GJGameLevel*);

    std::string buildInfoText();
    void /* TodoReturn */ buildStatsText();
    void /* TodoReturn */ decodeProgress(std::string const&);
    bool init(GJGameLevel*);
    void onClose(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void /* TodoReturn */ updatePage();

};
