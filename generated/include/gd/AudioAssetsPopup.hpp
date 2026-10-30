#pragma once

class CustomSongWidget;
class cocos2d;

class AudioAssetsPopup : public SetupTriggerPopup, public MusicDownloadDelegate {
public:
    virtual ~AudioAssetsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void musicActionFinished(GJMusicAction);

    static AudioAssetsPopup* create(int, int, std::string const&, std::string const&);

    bool init(int songID, int officialSongID, std::string const& songList, std::string const& sfxList);

    CustomSongWidget* m_customSongWidget;

};
