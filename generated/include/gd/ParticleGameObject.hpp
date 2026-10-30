#pragma once

class GJBaseGameLayer;
class GameObject;
class cocos2d;

class ParticleGameObject : public EnhancedGameObject {
public:
    virtual ~ParticleGameObject();
    virtual bool init();
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scale);
    virtual void setRotation(float rotation);
    virtual void setRotationX(float rotationX);
    virtual void setRotationY(float rotationY);
    virtual void setChildColor(cocos2d::ccColor3B const& color);
    virtual void customSetup();
    virtual void addMainSpriteToParent(bool reorder);
    virtual void resetObject();
    virtual void deactivateObject(bool deactivate);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void claimParticle();
    virtual void unclaimParticle();
    virtual void particleWasActivated();
    virtual void setObjectColor(cocos2d::ccColor3B const& color);
    virtual void blendModeChanged();
    virtual void updateParticleColor(cocos2d::ccColor3B const& color);
    virtual void updateParticleOpacity(unsigned char opacity);
    virtual void updateMainParticleOpacity(unsigned char opacity);
    virtual void updateSecondaryParticleOpacity(unsigned char opacity);
    virtual void updateSyncedAnimation(float totalTime, int frameIndex);
    virtual void updateAnimateOnTrigger(bool animate);

    static ParticleGameObject* create();

    ParticleGameObject();
    void applyParticleSettings(cocos2d::CCParticleSystemQuad* particle);
    void createAndAddCustomParticle();
    void createParticlePreviewArt();
    void setParticleString(std::string str);
    void updateParticle();
    void updateParticleAngle(float angle, cocos2d::CCParticleSystemQuad* particle);
    void updateParticlePreviewArtOpacity(float opacity);
    void updateParticleScale(float scale);
    void updateParticleStruct();

    std::string m_particleData;
    bool m_updatedParticleData;
    cocos2d::ParticleStruct m_particleStruct;
    bool m_hasUniformObjectColor;
    int m_popupPage;
    bool m_shouldQuickStart;
    float m_respawnResult;
    bool m_startingRespawn;
    bool m_notPreviewing;

};
