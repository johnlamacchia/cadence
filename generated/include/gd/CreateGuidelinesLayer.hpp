#pragma once

class CCTextInputNode;
class CustomSongDelegate;
class cocos2d;

class CreateGuidelinesLayer : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual void update(float dt);
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);
    virtual void playMusic();
    virtual void registerTouch();
    virtual void onInfo(cocos2d::CCObject* sender);
    virtual void onRecord(cocos2d::CCObject* sender);
    virtual void recordingDidStop();

    static CreateGuidelinesLayer* create(CustomSongDelegate* delegate, AudioGuidelinesType type);

    CreateGuidelinesLayer();
    ~CreateGuidelinesLayer();
    void doClearGuidelines();
    std::string getMergedRecordString(std::string str1, std::string str2);
    bool init(CustomSongDelegate* delegate, AudioGuidelinesType type);
    void onClearGuidelines(cocos2d::CCObject* sender);
    void onStop(cocos2d::CCObject* sender);
    void toggleItems(bool visible);

    CustomSongDelegate* m_delegate;
    CCTextInputNode* m_offsetInput;
    cocos2d::CCArray* m_nonRecordingObjects;
    cocos2d::CCArray* m_recordingObjects;
    cocos2d::CCLabelBMFont* m_infoLabel;
    cocos2d::CCSprite* m_circleSprite;
    float m_elapsed;
    bool m_recording;
    bool m_unk2b5;
    int m_guidelines;
    std::string m_guidelineString;

};
