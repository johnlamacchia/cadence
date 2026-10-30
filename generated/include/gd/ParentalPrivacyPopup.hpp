#pragma once

class GJUserScore;

class ParentalPrivacyPopup : public UploadActionPopup, public GJAccountSettingsDelegate, public UserInfoDelegate {
public:
    virtual ~ParentalPrivacyPopup();
    virtual void getUserInfoFinished(GJUserScore*);
    virtual void getUserInfoFailed(int);
    virtual void updateSettingsFinished();
    virtual void updateSettingsFailed();

    static ParentalPrivacyPopup* create(bool);

    void /* TodoReturn */ checkPending();
    bool init(bool);
    void /* TodoReturn */ snapshotIsForOtherAccount();
    void /* TodoReturn */ startWithScore(GJUserScore*);

};
