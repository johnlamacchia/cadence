#pragma once

class GJGameLevel;
class GJWorldNode;
class LevelPage;
class cocos2d;

class WorldLevelPage : public FLAlertLayer {
public:
    virtual void keyBackClicked();
    virtual void show();

    static WorldLevelPage* create(GJGameLevel* level, GJWorldNode* node);

    bool init(GJGameLevel* level, GJWorldNode* node);
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);
    void onSong(cocos2d::CCObject* sender);

    GJWorldNode* m_worldNode;
    GJGameLevel* m_level;

};
