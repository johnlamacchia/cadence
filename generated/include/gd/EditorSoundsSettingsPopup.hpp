#pragma once

class EditorSoundsLayer;
class cocos2d;

class EditorSoundsSettingsPopup : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~EditorSoundsSettingsPopup();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static EditorSoundsSettingsPopup* create(EditorSoundsLayer*);

    bool init(EditorSoundsLayer*);
    void onClearAll(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onDownloadAll(cocos2d::CCObject* sender);

};
