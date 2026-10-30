#pragma once

class cocos2d;

class FindBPMLayer : public CreateGuidelinesLayer {
public:
    virtual ~FindBPMLayer();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void playMusic();
    virtual void registerTouch();
    virtual void onInfo(cocos2d::CCObject* sender);
    virtual void onRecord(cocos2d::CCObject* sender);
    virtual void recordingDidStop();

    static FindBPMLayer* create(int songID);

    void calculateBPM();
    bool init(int songID);

    int m_songID;
    cocos2d::CCLabelBMFont* m_bpmLabel;
    float m_startOffset;
    int m_beats;
    int m_beatsPerMinute;

};
