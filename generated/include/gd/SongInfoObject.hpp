#pragma once

class DS_Dictionary;
class GJLinkObject;
class cocos2d;

class SongInfoObject : public cocos2d::CCNode {
public:
    virtual ~SongInfoObject();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();

    static SongInfoObject* create(cocos2d::CCDictionary* dict);
    static SongInfoObject* create(int songID);
    static SongInfoObject* create(int songID, std::string songName, std::string artistName, int artistID, float fileSize, std::string songUrl, int nongType, std::string extraArtistIDs, bool isNew, int libraryOrder, int priority);
    static SongInfoObject* createWithCoder(DS_Dictionary* dict);

    void addArtistLink(GJLinkObject const& link);
    void addSongLink(GJLinkObject const& link);
    void addTags(std::string tags);
    bool containsTag(int tag);
    void copyValues(SongInfoObject* object);
    std::vector<GJLinkObject> getArtistLinks();
    std::string getArtistNames(int unused);
    int getExtraArtistCount();
    std::vector<GJLinkObject> getSongLinks();
    std::string getTagsString(bool shortTags);
    bool init(int songID, std::string songName, std::string artistName, int artistID, float fileSize, std::string songUrl, int nongType, std::string extraArtistIDs, bool isNew, int libraryOrder, int priority);
    void setArtistLinks(std::string const& links);
    void setSongLinks(std::string const& links);
    void updateArtists(std::string artists);

    int m_songID;
    std::string m_songName;
    std::string m_artistName;
    std::string m_songUrl;
    int m_artistID;
    float m_fileSize;
    int m_nongType;
    std::string m_extraArtists;
    bool m_unloaded;
    bool m_verified;
    bool m_isBlocked;
    int m_priority;
    int m_duration;
    int m_BPM;
    bool m_isNew;
    int m_libraryOrder;
    int m_songOrder;
    std::string m_extraArtistNames;
    std::unordered_set<int> m_artistIDs;
    std::string m_songLinksString;
    std::string m_artistLinksString;
    std::vector<GJLinkObject> m_songLinks;
    std::vector<GJLinkObject> m_artistLinks;
    std::set<int> m_tags;
    std::string m_longTagsString;
    std::string m_shortTagsString;
    std::string m_songNameLower;
    std::string m_artistNameLower;

};
