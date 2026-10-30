#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class GJComment;
class GJEditorSoundConfig;
class GJFriendRequest;
class GJGameLevel;
class GJLevelList;
class GJMessageThread;
class GJThreadMessage;
class GJUserMessage;
class GJUserScore;
class ReportPopupConfig;
class cocos2d;

class ReportPopup : public SetupTriggerPopup, public FLAlertLayerProtocol {
public:
    virtual ~ReportPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static ReportPopup* create(ReportPopupConfig const&);

    void /* TodoReturn */ buildReasonRows(std::vector<int> const&, cocos2d::CCMenu*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler, float, float, float, float, float, float, std::vector<CCMenuItemToggler*>&, int, float);
    void /* TodoReturn */ contentTypeColor(int);
    void /* TodoReturn */ contentTypeName(int);
    void /* TodoReturn */ createForComment(GJComment*);
    void /* TodoReturn */ createForFriendRequest(GJFriendRequest*);
    void /* TodoReturn */ createForLevel(GJGameLevel*);
    void /* TodoReturn */ createForList(GJLevelList*);
    void /* TodoReturn */ createForMessage(GJUserMessage*);
    void /* TodoReturn */ createForSoundConfig(GJEditorSoundConfig*);
    void /* TodoReturn */ createForThread(GJMessageThread*, GJThreadMessage*, std::string const&);
    void /* TodoReturn */ createForUser(GJUserScore*);
    bool init(ReportPopupConfig const&);
    void onReasonToggle(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void /* TodoReturn */ reasonIndicesForTextModeration();
    void /* TodoReturn */ reasonIndicesForType(int);
    void /* TodoReturn */ reasonIndicesForUploadModeration();
    void /* TodoReturn */ reasonLabel(int);
    void /* TodoReturn */ reasonShortLabel(int);
    void /* TodoReturn */ selectedReasons();
    void /* TodoReturn */ surfaceName(int);

};
