#pragma once

class GJHttpResult : public cocos2d::CCNode {
public:
    virtual ~GJHttpResult();

    static GJHttpResult* create(bool success, std::string response, std::string tag, GJHttpType type);

    GJHttpResult();
    bool init(bool success, std::string response, std::string tag, GJHttpType type);

    bool m_success;
    std::string m_response;
    std::string m_requestTag;
    GJHttpType m_httpType;

};
