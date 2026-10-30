#pragma once

class CCURLObject : public cocos2d::CCObject {
public:
    virtual ~CCURLObject();

    static CCURLObject* create(std::string name, std::string url);

    bool init(std::string name, std::string url);

    std::string m_name;
    std::string m_url;

};
