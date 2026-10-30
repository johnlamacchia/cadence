#pragma once

class LevelRateInfoDelegate {
public:
    virtual void rateInfoFinished(int id, std::string info);
    virtual void rateInfoFailed(int id, int response);

};
