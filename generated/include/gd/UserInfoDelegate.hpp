#pragma once

class GJUserScore;

class UserInfoDelegate {
public:
    virtual void getUserInfoFinished(GJUserScore* score);
    virtual void getUserInfoFailed(int id);
    virtual void userInfoChanged(GJUserScore* score);

};
