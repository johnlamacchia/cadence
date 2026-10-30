#pragma once

class GJLinkObject;
class InfoLayer;
class SongInfoObject;
class cocos2d;

class SongInfoLayer : public FLAlertLayer {
public:
    virtual ~SongInfoLayer();
    virtual void keyBackClicked();

    static SongInfoLayer* create(SongInfoObject*);
    static SongInfoLayer* create(int artistID);
    static SongInfoLayer* create(std::string, std::string, std::vector<GJLinkObject>, std::vector<GJLinkObject>, int, std::string, int);

    void /* TodoReturn */ addLinkRows(std::vector<GJLinkObject> const&, float, int);
    bool init(std::string, std::string, std::vector<GJLinkObject>, std::vector<GJLinkObject>, int, std::string, int);
    void onBPM(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onCopySongID(cocos2d::CCObject* sender);
    void onLink(cocos2d::CCObject* sender);
    void onMenuMusic(cocos2d::CCObject* sender);
    void onPracticeMusic(cocos2d::CCObject* sender);
    void onSongUsage(cocos2d::CCObject* sender);
    void showCustomMusicUnlockInfo();

    std::string m_songName;
    std::string m_artistName;
    int m_songID;
    int m_musicCustomizerMessageIndex;
    std::vector<GJLinkObject> m_artistLinks;

};
