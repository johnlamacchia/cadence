#pragma once

class GJBaseGameLayer;
class GameObject;
class cocos2d;

class SpecialAnimGameObject : public EnhancedGameObject {
public:
    virtual ~SpecialAnimGameObject();
    virtual void resetObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void updateMainColor(cocos2d::ccColor3B const& color);
    virtual void updateSecondaryColor(cocos2d::ccColor3B const& color);
    virtual void updateSyncedAnimation(float totalTime, int frameIndex);

    static SpecialAnimGameObject* create(char const* frame);

    SpecialAnimGameObject();
    bool init(char const* frame);

    bool m_skipMainColorUpdate;
    bool m_skipSecondaryColorUpdate;

};
