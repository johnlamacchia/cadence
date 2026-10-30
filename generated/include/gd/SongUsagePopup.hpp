#pragma once

class SongUsageEntry;
class cocos2d;

class SongUsagePopup : public FLAlertLayer, public SongUsageDelegate {
public:
    virtual ~SongUsagePopup();
    virtual void keyBackClicked();
    virtual void songUsageFinished(int, std::string);
    virtual void songUsageFailed(int);

    static SongUsagePopup* create(int, std::string const&, std::string const&);

    void /* TodoReturn */ buildStatRow(std::string const*, std::string const*, int, float);
    void /* TodoReturn */ buildUsageGrid(std::vector<SongUsageEntry> const&);
    bool init(int, std::string const&, std::string const&);
    void /* TodoReturn */ layoutPopup(float);
    void onClose(cocos2d::CCObject* sender);
    void /* TodoReturn */ showStatus(char const*);

};
