#pragma once

class AdminActionConfirmDelegate {
public:
    virtual void adminActionConfirmed(int, std::vector<int> const&, std::string const&);

};
