#pragma once

class DS_Dictionary;
class GJSmartPrefab;
class GameObject;
class SmartGameObject;
class SmartPrefabResult;
class cocos2d;

class GJSmartTemplate : public cocos2d::CCObject {
public:
    virtual ~GJSmartTemplate();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();

    static void applyTransformationsForType(SmartBlockType type, cocos2d::CCSprite* sprite);
    static GJSmartTemplate* create();
    static GJSmartTemplate* createWithCoder(DS_Dictionary* dict);
    static SmartBlockType flipBlockType(SmartBlockType type, bool flipX, bool flipY);
    static SmartBlockType flipBlockTypeX(SmartBlockType type);
    static SmartBlockType flipBlockTypeY(SmartBlockType type);
    static std::string flipKey(std::string key, bool flipX, bool flipY);
    static cocos2d::CCDictionary* generateRemapDict();
    static std::string getSimplifiedKey(std::string key);
    static SmartBlockType getSimplifiedType(SmartBlockType type, bool& changed);
    static std::string getVerySimplifiedKey(std::string key);
    static std::string keyFromNeighbors(SmartBlockType type, SmartBlockType left, SmartBlockType right, SmartBlockType top, SmartBlockType bottom, SmartBlockType topLeft, SmartBlockType topRight, SmartBlockType bottomLeft, SmartBlockType bottomRight);
    static std::string keyFromNeighbors(cocos2d::CCPoint position, SmartGameObject* object, SmartGameObject* left, SmartGameObject* right, SmartGameObject* top, SmartGameObject* bottom, SmartGameObject* topLeft, SmartGameObject* topRight, SmartGameObject* bottomLeft, SmartGameObject* bottomRight);
    static std::string keyFromNeighborsOld(bool left, bool right, bool top, bool bottom, bool topLeft, bool topRight, bool bottomLeft, bool bottomRight);
    static cocos2d::CCPoint offsetForDir(GJSmartDirection direction, int key);
    static cocos2d::CCPoint offsetForObject(SmartGameObject* object);
    static cocos2d::CCPoint offsetForType(SmartBlockType type);
    static SmartBlockType rotateBlockType(SmartBlockType type, int degrees);
    static SmartBlockType rotateBlockType90(SmartBlockType type);
    static std::string rotateKey(std::string key, int degrees);
    static bool shouldDiscardObject(SmartBlockType type, GJSmartDirection direction);
    static SmartBlockType smartObjectToType(SmartGameObject* object, cocos2d::CCPoint point);
    static int smartTypeToObjectKey(SmartBlockType type);

    void dataLoaded(DS_Dictionary* dict);
    std::string getNoCornerKey(std::string key);
    GJSmartPrefab* getPrefab(std::string key, bool flipX, bool flipY);
    GJSmartPrefab* getPrefab(cocos2d::CCPoint position, SmartGameObject* object, SmartGameObject* left, SmartGameObject* right, SmartGameObject* top, SmartGameObject* bottom, SmartGameObject* topLeft, SmartGameObject* topRight, SmartGameObject* bottomLeft, SmartGameObject* bottomRight);
    cocos2d::CCArray* getPrefabs(std::string key);
    GJSmartPrefab* getPrefabWithID(std::string key, int id);
    GJSmartPrefab* getRandomPrefab(std::string key);
    void getTemplateState(std::vector<SmartPrefabResult>& results);
    int getTotalChanceForPrefab(std::string key);
    bool init();
    bool isUnrequired(std::string key);
    std::string logTemplateStatus(bool unused);
    void removePrefab(std::string prefabKey, int prefabID);
    void resetScannedPrefabs();
    void savePrefab(std::string key, std::string data);
    void saveRemap(std::string key);
    void saveRemapToDict(std::string key, std::string remap, cocos2d::CCDictionary* dict);
    SmartPrefabResult scanForPrefab(std::string key);

    cocos2d::CCDictionary* m_prefabArrays;
    cocos2d::CCDictionary* m_remapDict1;
    cocos2d::CCDictionary* m_remapDict2;
    cocos2d::CCDictionary* m_remapDict3;
    cocos2d::CCDictionary* m_remapDict4;
    cocos2d::CCDictionary* m_remapDict5;
    cocos2d::CCDictionary* m_remapDict6;
    std::map<std::string, SmartPrefabResult> m_prefabResults;
    bool m_allowFlipX;
    bool m_allowFlipY;
    bool m_allowRotation;
    bool m_ignoreCorners;
    std::string m_templateName;
    std::string m_unk0a8;
    int m_nameIndex;
    int m_prefabIndex;

};
