#pragma once

class DS_Dictionary;
class GJAccountBackupDelegate;
class GJAccountDelegate;
class GJAccountLoginDelegate;
class GJAccountRegisterDelegate;
class GJAccountSettingsDelegate;
class GJAccountSyncDelegate;
class cocos2d;

class GJAccountManager : public cocos2d::CCNode {
public:
    virtual ~GJAccountManager();
    virtual bool init();

    static GJAccountManager* get();
    static GJAccountManager* sharedState();

    void addDLToActive(char const* tag);
    void addDLToActive(char const* tag, cocos2d::CCObject* object);
    bool backupAccount(std::string url);
    void dataLoaded(DS_Dictionary* dict);
    void encodeDataTo(DS_Dictionary* dict);
    void firstSetup();
    bool getAccountBackupURL();
    bool getAccountSyncURL();
    cocos2d::CCObject* getDLObject(char const* tag);
    std::string getShaPassword(std::string password);
    void handleIt(bool success, std::string response, std::string tag, GJHttpType type);
    void handleItDelayed(bool success, std::string response, std::string tag, GJHttpType type);
    void handleItND(cocos2d::CCNode* node, void* data);
    bool isDLActive(char const* tag);
    void linkToAccount(std::string username, std::string gjp2, int accountID, int userID);
    void loginAccount(std::string username, std::string gjp2);
    void onBackupAccountCompleted(std::string response, std::string tag);
    void onGetAccountBackupURLCompleted(std::string response, std::string tag);
    void onGetAccountSyncURLCompleted(std::string response, std::string tag);
    void onLoginAccountCompleted(std::string response, std::string tag);
    void /* TodoReturn */ onOpenRegisterCompleted(std::string, std::string);
    void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    void onRegisterAccountCompleted(std::string response, std::string tag);
    void onSyncAccountCompleted(std::string response, std::string tag);
    void onUpdateAccountSettingsCompleted(std::string response, std::string tag);
    void /* TodoReturn */ openRegister();
    void ProcessHttpRequest(std::string url, std::string params, std::string tag, GJHttpType type);
    void registerAccount(std::string response, std::string tag, std::string data);
    void removeDLFromActive(char const* tag);
    void /* TodoReturn */ showAccountNeededAlert(char const*);
    bool syncAccount(std::string url);
    void unlinkFromAccount();
    void /* TodoReturn */ updateAccountSettings(int, int, int, int, int, std::string, std::string, std::string, std::string, std::string, std::string, std::string);

    cocos2d::CCDictionary* m_activeDownloads;
    std::string m_username;
    int m_accountID;
    int m_unkInt1;
    int m_unkInt2;
    std::string m_GJP2;
    GJAccountRegisterDelegate* m_accountRegisterDelegate;
    GJAccountLoginDelegate* m_accountLoginDelegate;
    GJAccountDelegate* m_accountDelegate;
    GJAccountBackupDelegate* m_backupDelegate;
    GJAccountSyncDelegate* m_syncDelegate;
    GJAccountSettingsDelegate* m_accountSettingsDelegate;
    int m_gameManagerSize;
    int m_localLevelsSize;
    std::string m_password;
    std::string m_hash;

};
