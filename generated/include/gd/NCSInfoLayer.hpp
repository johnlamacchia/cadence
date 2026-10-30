#pragma once

class CustomSongLayer;
class InfoLayer;
class cocos2d;

class NCSInfoLayer : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~NCSInfoLayer();
    virtual void keyBackClicked();

    static NCSInfoLayer* create(CustomSongLayer* layer);

    NCSInfoLayer();
    bool init(CustomSongLayer* layer);
    void onClose(cocos2d::CCObject* sender);
    void onLibrary(cocos2d::CCObject* sender);
    void onNCS(cocos2d::CCObject* sender);
    void onNCSIO(cocos2d::CCObject* sender);
    void onNCSUsage(cocos2d::CCObject* sender);

    CustomSongLayer* m_songLayer;

};
