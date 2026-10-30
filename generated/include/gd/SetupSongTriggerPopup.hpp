#pragma once

class EffectGameObject;
class GameObject;
class SongInfoObject;
class SongSelectNode;
class cocos2d;

class SetupSongTriggerPopup : public SetupAudioTriggerPopup, public MusicDownloadDelegate, public SongPlaybackDelegate {
public:
    virtual ~SetupSongTriggerPopup();
    virtual void pageChanged();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual void onPlayback(SongInfoObject* object);

    static SetupSongTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onResetSongTime(cocos2d::CCObject* sender);
    void onSavePlaybackPos(cocos2d::CCObject* sender);
    void updateApplyPrepare(bool hideAll);
    void updateLength();
    void updateSongTimeSlider();

    SongSelectNode* m_selectNode;

};
