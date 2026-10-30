#pragma once

class cocos2d;

class DS_Dictionary {
public:
    static void copyFile(char const*, char const*);

    DS_Dictionary();
    void addBoolValuesToMapForKey(std::map<std::string, bool>&, char const*, bool);
    void addBoolValuesToMapForKeySpecial(std::map<std::string, bool>&, char const*, bool);
    void checkCompatibility();
    std::string cleanStringWhiteSpace(std::string const&);
    cocos2d::CCObject* decodeObjectForKey(char const*, bool, int);
    std::vector<std::string> getAllKeys();
    cocos2d::CCArray* getArrayForKey(char const*, bool);
    bool getBoolForKey(char const*);
    cocos2d::CCDictionary* getDictForKey(char const*, bool);
    float getFloatForKey(char const*);
    unsigned int getIndexOfKey(char const*);
    unsigned int getIndexOfKeyWithClosestAlphaNumericalMatch(char const*);
    int getIntegerForKey(char const*);
    std::string getKey(unsigned int);
    unsigned int getNumKeys();
    cocos2d::CCObject* getObjectForKey(char const*);
    std::vector<cocos2d::CCRect> getRectArrayForKey(char const*);
    cocos2d::CCRect getRectForKey(char const*);
    std::vector<std::string> getStringArrayForKey(char const*);
    std::string getStringForKey(char const*);
    std::vector<cocos2d::CCPoint> getVec2ArrayForKey(char const*);
    cocos2d::CCPoint getVec2ForKey(char const*);
    bool loadRootSubDictFromCompressedFile(char const*, bool);
    bool loadRootSubDictFromFile(char const*);
    bool loadRootSubDictFromString(std::string const&);
    bool rectFromString(std::string const&, cocos2d::CCRect&);
    void removeAllKeys();
    void removeKey(char const*);
    void removeKey(unsigned int);
    bool saveRootSubDictToCompressedFile(char const*, bool);
    bool saveRootSubDictToFile(char const*);
    std::string saveRootSubDictToString();
    void setArrayForKey(char const*, cocos2d::CCArray*);
    void setBoolForKey(char const*, bool);
    void setBoolForKey(char const*, bool, bool);
    void setBoolMapForKey(char const*, std::map<std::string, bool>&);
    void setDictForKey(char const*, cocos2d::CCDictionary*);
    void setFloatForKey(char const*, float);
    void setFloatForKey(char const*, float, bool);
    void setIntegerForKey(char const*, int);
    void setIntegerForKey(char const*, int, bool);
    void setObjectForKey(char const*, cocos2d::CCObject*);
    void setRectArrayForKey(char const*, std::vector<cocos2d::CCRect> const&);
    void setRectArrayForKey(char const*, std::vector<cocos2d::CCRect> const&, bool);
    void setRectForKey(char const*, cocos2d::CCRect const&);
    void setRectForKey(char const*, cocos2d::CCRect const&, bool);
    void setStringArrayForKey(char const*, std::vector<std::string> const&);
    void setStringArrayForKey(char const*, std::vector<std::string> const&, bool);
    void setStringForKey(char const*, std::string const&);
    void setStringForKey(char const*, std::string const&, bool);
    void setSubDictForKey(char const*);
    void setSubDictForKey(char const*, bool, bool);
    void setVec2ArrayForKey(char const*, std::vector<cocos2d::CCPoint> const&);
    void setVec2ArrayForKey(char const*, std::vector<cocos2d::CCPoint> const&, bool);
    void setVec2ForKey(char const*, cocos2d::CCPoint const&);
    void setVec2ForKey(char const*, cocos2d::CCPoint const&, bool);
    void split(std::string const&, char const*, std::vector<std::string>&);
    bool splitWithForm(std::string const&, std::vector<std::string>&);
    void stepBackToRootSubDict();
    bool stepIntoSubDictWithKey(char const*);
    void stepOutOfSubDict();
    bool vec2FromString(std::string const&, cocos2d::CCPoint&);

};
