#pragma once

class GJBaseGameLayer;
class cocos2d;

class EndPortalObject : public GameObject {
public:
    virtual ~EndPortalObject();
    virtual bool init();
    virtual void setPosition(cocos2d::CCPoint const& position);
    virtual void setVisible(bool visible);

    static EndPortalObject* create();

    EndPortalObject();
    cocos2d::CCPoint getSpawnPos();
    void triggerObject(GJBaseGameLayer* layer);
    void updateColors(cocos2d::ccColor3B color);
    void updateEndPos(bool updateParticle);

    cocos2d::CCSprite* m_gradientBar;
    bool m_flippedX;
    bool m_startPosHeightRelated;

};
