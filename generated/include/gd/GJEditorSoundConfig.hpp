#pragma once

class DS_Dictionary;
class cocos2d;

class GJEditorSoundConfig : public cocos2d::CCNode {
public:
    virtual ~GJEditorSoundConfig();
    virtual void encodeWithCoder(DS_Dictionary*);
    virtual bool init();

    static GJEditorSoundConfig* create();
    static GJEditorSoundConfig* create(cocos2d::CCDictionary*);

    void /* TodoReturn */ clearServerState();
    void /* TodoReturn */ copyServerFields(GJEditorSoundConfig*);
    void /* TodoReturn */ createWithCoder(DS_Dictionary*);
    void /* TodoReturn */ dataLoaded(DS_Dictionary*);
    void /* TodoReturn */ getUnpackedSoundDesc();
    void /* TodoReturn */ showSoundInfo();

};
