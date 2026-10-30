#pragma once

class cocos2d;

class OnlineListDelegate {
public:
    virtual void loadListFinished(cocos2d::CCArray* objects, char const* key);
    virtual void loadListFailed(char const* key);
    virtual void setupPageInfo(std::string info, char const* key);

};
