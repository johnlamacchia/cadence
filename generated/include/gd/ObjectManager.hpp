#pragma once

class cocos2d;

class ObjectManager : public cocos2d::CCNode {
public:
    virtual ~ObjectManager();
    virtual bool init();

    static ObjectManager* instance();
    static cocos2d::CCDictionary* replaceAllOccurencesOfString(cocos2d::CCString* target, cocos2d::CCString* replacement, cocos2d::CCDictionary* dict);

    bool animLoaded(char const* anim);
    cocos2d::CCDictionary* getDefinition(char const* definitionKey);
    cocos2d::CCDictionary* getGlobalAnimCopy(char const* anim);
    void loadCopiedAnimations();
    void loadCopiedSets();
    void purgeObjectManager();
    void setLoaded(char const* objectName);
    void setup();

    cocos2d::CCDictionary* m_objectDefinitions;
    cocos2d::CCDictionary* m_loadedAnimations;

};
