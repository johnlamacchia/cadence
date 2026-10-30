#pragma once

class StatsObject : public cocos2d::CCObject {
public:
    virtual ~StatsObject();

    static StatsObject* create(char const* key, int value);

    bool init(char const* key, int value);

    const char* m_key;
    int m_value;

};
