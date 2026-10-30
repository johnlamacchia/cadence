#pragma once

class AudioLineGuideGameObject;
class EffectGameObject;
class GameObject;
class LevelEditorLayer;
class cocos2d;

class DrawGridLayer : public cocos2d::CCLayer {
public:
    virtual ~DrawGridLayer();
    virtual void update(float dt);
    virtual void draw();

    static DrawGridLayer* create(cocos2d::CCNode* parent, LevelEditorLayer* layer);

    void addAudioLineObject(AudioLineGuideGameObject* object);
    void addToEffects(EffectGameObject* object);
    void addToGuides(GameObject* object);
    void addToSpeedObjects(EffectGameObject* object);
    cocos2d::CCPoint getPortalMinMax(GameObject* object);
    bool init(cocos2d::CCNode* parent, LevelEditorLayer* layer);
    void loadTimeMarkers(std::string markers);
    cocos2d::CCPoint posForTime(float time);
    void postUpdate();
    void removeAudioLineObject(AudioLineGuideGameObject* object);
    void removeFromEffects(EffectGameObject* object);
    void removeFromGuides(GameObject* object);
    void removeFromSpeedObjects(EffectGameObject* object);
    void sortSpeedObjects();
    float timeForPos(cocos2d::CCPoint position, int order, int channel, bool songTriggers, bool ignoreWarp, bool ignoreRotate, int id);
    void updateMusicGuideTime(float time);
    void updateTimeMarkers();

    std::array<cocos2d::CCPoint, 400>* m_pointArray1;
    std::array<cocos2d::CCPoint, 400>* m_pointArray2;
    std::array<cocos2d::CCPoint, 400>* m_pointArray3;
    std::unordered_map<int, AudioLineGuideGameObject*> m_audioLineObjects;
    float m_musicTime;
    float m_playbackTime;
    float m_oldPlaybackTime;
    float m_playbackX;
    float m_playbackY;
    bool m_sortEffects;
    LevelEditorLayer* m_editorLayer;
    std::string m_timeMarkerString;
    cocos2d::CCNode* m_objectLayer;
    cocos2d::CCArray* m_timeMarkers;
    cocos2d::CCArray* m_effectGameObjects;
    cocos2d::CCArray* m_guideObjects;
    cocos2d::CCArray* m_speedObjects;
    double m_beatWidth;
    float m_currentSpeed;
    float m_slowSpeed;
    float m_normalSpeed;
    float m_fastSpeed;
    float m_fasterSpeed;
    float m_fastestSpeed;
    bool m_updateTimeMarkers;
    bool m_updateSpeedObjects;
    float m_gridSize;

};
