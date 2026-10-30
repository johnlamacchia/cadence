#pragma once

class FLAlertLayer;
class GJGameLevel;
class LevelOptionsLayer;
class OptionsLayer;

class GameLevelOptionsLayer : public GJOptionsLayer, public FLAlertLayerProtocol {
public:
    virtual ~GameLevelOptionsLayer();
    virtual void setupOptions();
    virtual void didToggle(int tag);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static GameLevelOptionsLayer* create(GJGameLevel* level);

    bool init(GJGameLevel* level);
    void /* TodoReturn */ toggleLDM();

    GJGameLevel* m_level;

};
