#pragma once

class cocos2d;

class GJMGLayer : public cocos2d::CCLayer {
public:
    virtual ~GJMGLayer();
    virtual void draw();
    virtual void showGround();

    static GJMGLayer* create(int index);
    static float defaultYOffsetForBG2(int index);

    GJMGLayer();
    void deactivateGround();
    bool init(int index);
    void loadGroundSprites(int count, bool ground1);
    float scaleGround(float scale);
    void toggleVisible01(bool visible);
    void toggleVisible02(bool visible);
    void updateGroundColor(cocos2d::ccColor3B color, bool ground1);
    void updateGroundOpacity(unsigned char opacity, bool ground1);
    void updateGroundPos(cocos2d::CCPoint pos);
    void updateGroundWidth(bool useThis);
    void updateMG01Blend(bool blend);
    void updateMG02Blend(bool blend);

    cocos2d::CCSprite* m_ground1Sprite;
    cocos2d::CCSprite* m_ground2Sprite;
    float m_textureWidth;
    void* m_unk1b0;
    bool m_showGround;
    float m_groundWidth;
    bool m_showGround1;
    bool m_showGround2;
    float m_unk1c4;
    bool m_cameraRotated;
    bool m_blendMG1;
    bool m_blendMG2;
    cocos2d::CCSpriteBatchNode* m_mg1BatchNode;
    cocos2d::CCSpriteBatchNode* m_mg2BatchNode;
    float m_groundScale;
    float m_ground2Offset;

};
