#pragma once

class FLAlertLayerProtocol;
class GJGameLevel;

class DailyRecallUnlockPopup : public FLAlertLayer {
public:
    virtual ~DailyRecallUnlockPopup();

    static DailyRecallUnlockPopup* create(FLAlertLayerProtocol*, GJGameLevel*);

    DailyRecallUnlockPopup();
    bool init(FLAlertLayerProtocol*, GJGameLevel*);

};
