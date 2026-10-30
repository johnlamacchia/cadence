#pragma once

class GJAccountRegisterDelegate {
public:
    virtual void registerAccountFinished();
    virtual void registerAccountFailed(AccountError errorType);

};
