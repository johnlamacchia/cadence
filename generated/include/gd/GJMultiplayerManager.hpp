#pragma once

class DS_Dictionary;
class GJMPDelegate;
class UploadActionDelegate;
class cocos2d;

class GJMultiplayerManager : public cocos2d::CCNode {
public:
    virtual ~GJMultiplayerManager();
    virtual bool init();

    static GJMultiplayerManager* sharedState();

    void addComment(std::string str, int lobbyID);
    void addDLToActive(char const* tag);
    void addDLToActive(char const* tag, cocos2d::CCObject* obj);
    void createAndAddComment(std::string text, int lobbyID);
    void dataLoaded(DS_Dictionary* dict);
    void encodeDataTo(DS_Dictionary* dict);
    bool exitLobby(int id);
    void firstSetup();
    std::string getBasePostString();
    cocos2d::CCObject* getDLObject(char const* tag);
    int getLastCommentIDForGame(int id);
    void handleIt(bool success, std::string response, std::string tag, GJHttpType type);
    void handleItDelayed(bool success, std::string response, std::string tag, GJHttpType type);
    void handleItND(cocos2d::CCNode* node, void* data);
    bool isDLActive(char const* tag);
    bool joinLobby(int id);
    void onExitLobbyCompleted(std::string response, std::string tag);
    void onJoinLobbyCompleted(std::string response, std::string tag);
    void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    void onUploadCommentCompleted(std::string response, std::string tag);
    void ProcessHttpRequest(std::string url, std::string params, std::string tag, GJHttpType type);
    void removeDLFromActive(char const* tag);
    bool uploadComment(std::string text, int lobbyID);

    cocos2d::CCDictionary* m_activeDownloads;
    cocos2d::CCDictionary* m_lobbyScores;
    cocos2d::CCDictionary* m_scoreArrays;
    cocos2d::CCDictionary* m_lobbyComments;
    GJMPDelegate* m_mpDelegate;
    UploadActionDelegate* m_uploadDelegate;

};
