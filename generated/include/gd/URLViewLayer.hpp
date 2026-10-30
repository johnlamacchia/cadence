#pragma once

class cocos2d;

class URLViewLayer : public GJDropDownLayer {
public:
    virtual ~URLViewLayer();

    static URLViewLayer* create(std::string title, cocos2d::CCArray* objects);

    bool init(std::string title, cocos2d::CCArray* objects);

    cocos2d::CCArray* m_urlObjects;

};
