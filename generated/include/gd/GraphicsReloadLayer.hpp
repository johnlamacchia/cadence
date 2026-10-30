#pragma once

class cocos2d;

class GraphicsReloadLayer : public cocos2d::CCLayer {
public:
    virtual ~GraphicsReloadLayer();

    static GraphicsReloadLayer* create(cocos2d::TextureQuality quality, cocos2d::CCSize resolution, bool fullscreen, bool borderless, bool fix, bool changedResolution);
    static cocos2d::CCScene* scene(cocos2d::TextureQuality quality, cocos2d::CCSize resolution, bool fullscreen, bool borderless, bool fix, bool changedResolution);

    GraphicsReloadLayer();
    bool init(cocos2d::TextureQuality quality, cocos2d::CCSize resolution, bool fullscreen, bool borderless, bool fix, bool changedResolution);
    void performReload();

    cocos2d::TextureQuality m_quality;
    cocos2d::CCSize m_resolution;
    bool m_fullscreen;
    bool m_borderless;
    bool m_fix;
    bool m_changedResolution;

};
