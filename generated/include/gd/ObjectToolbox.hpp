#pragma once

class cocos2d;

class ObjectToolbox : public cocos2d::CCNode {
public:
    virtual ~ObjectToolbox();
    virtual bool init();

    static ObjectToolbox* sharedState();

    cocos2d::CCArray* allKeys();
    float gridNodeSizeForKey(int key);
    const char* intKeyToFrame(int key);
    const char* perspectiveBlockFrame(int key);

    std::map<int, std::string> m_allKeys;

};
