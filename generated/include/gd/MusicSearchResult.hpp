#pragma once

class OptionsObject;
class cocos2d;

class MusicSearchResult : public cocos2d::CCObject, public OptionsObjectDelegate {
public:
    virtual void updateObjects(AudioSortType type);
    virtual void stateChanged(OptionsObject* object);

    static MusicSearchResult* create(GJSongType songType);

    MusicSearchResult();
    ~MusicSearchResult();
    cocos2d::CCArray* applyArtistFilters(cocos2d::CCArray* objects);
    cocos2d::CCArray* applyTagFilters(cocos2d::CCArray* objects);
    void /* TodoReturn */ applyViewFilter(cocos2d::CCArray*);
    void createArtistFilterObjects();
    void createTagFilterObjects();
    cocos2d::CCArray* getFilesMatchingSearch(cocos2d::CCArray* objects, std::string query);
    bool init(GJSongType songType);
    void updateFutureCount(cocos2d::CCArray*, cocos2d::CCArray*, cocos2d::CCArray*);
    void updateObjects();

    cocos2d::CCArray* m_filterObjects;
    cocos2d::CCArray* m_tagFilterObjects;
    cocos2d::CCArray* m_artistFilterObjects;
    cocos2d::CCArray* m_viewFilterObjects;
    int m_startIndex;
    int m_audioType;
    std::string m_searchQuery;
    AudioSortType m_sortType;
    bool m_tagFilter;
    bool m_artistFilter;
    bool m_matchAny;
    GJSongType m_songType;
    int m_viewFilter;
    std::vector<int> m_filterIDs;

};
