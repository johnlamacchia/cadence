#pragma once

class GJAccountLoginDelegate {
public:
    virtual void loginAccountFinished(int accountID, int userID);
    virtual void loginAccountFailed(AccountError errorType);

};
