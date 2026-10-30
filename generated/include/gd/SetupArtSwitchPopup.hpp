#pragma once

class ArtTriggerGameObject;
class GameObject;
class SelectArtLayer;
class cocos2d;

class SetupArtSwitchPopup : public SetupTriggerPopup, public SelectArtDelegate {
public:
    virtual ~SetupArtSwitchPopup();
    virtual void selectArtClosed(SelectArtLayer* layer);

    static SetupArtSwitchPopup* create(ArtTriggerGameObject* object, cocos2d::CCArray* objects, int id);

    bool init(ArtTriggerGameObject* object, cocos2d::CCArray* objects, int id);
    void onArt(cocos2d::CCObject* sender);

    SelectArtType m_artType;
    cocos2d::CCSprite* m_artSprite;

};
