#pragma once

class GJBaseGameLayer;
class GJGameLevel;
class SongInfoObject;
class cocos2d;

class LevelTools {
public:
    static int artistForAudio(int id);
    static std::string base64DecodeString(std::string str);
    static std::string base64EncodeString(std::string str);
    static cocos2d::CCDictionary* createStarPackDict();
    static std::string fbURLForArtist(int id);
    static int getAudioBPM(int id);
    static std::string getAudioFileName(int id);
    static std::string getAudioString(int id);
    static std::string getAudioTitle(int id);
    static bool getLastGameplayReversed();
    static bool getLastGameplayRotated();
    static float getLastTimewarp();
    static GJGameLevel* getLevel(int id, bool noString);
    static std::unordered_set<int> getLevelList();
    static SongInfoObject* getSongObject(int id);
    static void moveTriggerObjectsToArray(cocos2d::CCArray* objects, cocos2d::CCDictionary* objectsDict, int index);
    static std::string nameForArtist(int id);
    static std::string ngURLForArtist(int id);
    static float offsetBPMForTrack(int id);
    static cocos2d::CCPoint posForTime(float time, cocos2d::CCArray* objects, int speed, bool platformer, int& rotateChannel);
    static cocos2d::CCPoint posForTimeInternal(float time, cocos2d::CCArray* objects, int speed, bool platformer, bool ignoreMinorAxis, bool ignoreWarp, int& rotateChannel, int unused);
    static void sortChannelOrderObjects(cocos2d::CCArray* objects, cocos2d::CCDictionary* objectsDict, bool moveObjects);
    static void sortSpeedObjects(cocos2d::CCArray* objects, GJBaseGameLayer* layer);
    static float timeForPos(cocos2d::CCPoint position, cocos2d::CCArray* objects, int speed, int order, int channel, bool songTriggers, bool platformer, bool ignoreWarp, bool ignoreRotate, int id);
    static void toggleDebugLogging(bool enabled);
    static std::string urlForAudio(int id);
    static float valueForSpeedMod(int speed);
    static bool verifyLevelIntegrity(std::string str, int id);
    static std::string ytURLForArtist(int id);

};
