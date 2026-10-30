#pragma once

class GJSmartPrefab;

class SmartPrefabResult {
public:
    GJSmartPrefab* m_smartPrefab;
    std::string m_binaryKey;
    std::string m_prefabKey;
    int m_prefabCount;
    bool m_unrequired;
    int m_rotation;
    bool m_flipX;
    bool m_flipY;
    bool m_ignoreCorners;

};
