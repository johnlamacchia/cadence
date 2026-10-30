#pragma once

class cocos2d;

class GJMoreGamesLayer : public GJDropDownLayer {
public:
    virtual void customSetup();

    static GJMoreGamesLayer* create();

    cocos2d::CCArray* getMoreGamesList();

    cocos2d::CCArray* m_moreGamesList;

};
