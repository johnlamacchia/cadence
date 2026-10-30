#pragma once

class CCCircleAlert : public CCCircleWave {
public:
    static CCCircleAlert* create(float duration, float startRadius, float endRadius);

    bool init(float duration, float startRadius, float endRadius);

};
