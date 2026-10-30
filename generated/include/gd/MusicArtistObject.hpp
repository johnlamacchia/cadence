#pragma once

class GJLinkObject;

class MusicArtistObject : public cocos2d::CCObject {
public:
    virtual ~MusicArtistObject();

    static MusicArtistObject* create(int id, std::string name);

    bool init(int id, std::string name);

    int m_artistID;
    std::string m_artistName;
    std::vector<GJLinkObject> m_artistLinks;

};
