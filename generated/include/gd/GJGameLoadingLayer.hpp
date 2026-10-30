#pragma once

class GJGameLevel;
class LoadingLayer;

class GJGameLoadingLayer : public cocos2d::CCLayer {
public:
    virtual ~GJGameLoadingLayer();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();

    static GJGameLoadingLayer* create(GJGameLevel* level, bool editor);
    static GJGameLoadingLayer* transitionToLoadingLayer(GJGameLevel* level, bool editor);

    GJGameLoadingLayer();
    void gameLayerDidUnload();
    bool init(GJGameLevel* level, bool editor);
    void loadLevel();

    GJGameLevel* m_level;
    bool m_editor;

};
