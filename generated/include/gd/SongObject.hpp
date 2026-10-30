#pragma once

class SongObject : public cocos2d::CCObject {
public:
    virtual ~SongObject();

    static SongObject* create(int audioID);

    SongObject();
    bool init(int audioID);

    int m_audioID;

};
