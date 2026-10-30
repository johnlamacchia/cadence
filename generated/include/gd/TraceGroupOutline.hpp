#pragma once

class cocos2d;

class TraceGroupOutline {
public:
    uint64_t m_version;
    std::vector<cocos2d::CCPoint> m_points;
    std::vector<cocos2d::CCPoint> m_points2;

};
