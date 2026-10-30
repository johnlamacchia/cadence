#pragma once

class cocos2d;

class OBB2D : public cocos2d::CCNode {
public:
    virtual ~OBB2D();

    static OBB2D* create(cocos2d::CCPoint center, float width, float height, float rotationAngle);

    void calculateWithCenter(cocos2d::CCPoint center, float width, float height, float rotationAngle);
    void computeAxes();
    cocos2d::CCRect getBoundingRect();
    bool init(cocos2d::CCPoint center, float width, float height, float rotationAngle);
    void orderCorners();
    bool overlaps(OBB2D* other);
    bool overlaps1Way(OBB2D* other);

    std::array<cocos2d::CCPoint, 4> m_corners;
    std::array<cocos2d::CCPoint, 4> m_positions;
    std::array<cocos2d::CCPoint, 4> m_edges;
    std::array<double, 2> m_projections;
    cocos2d::CCPoint m_center;

};
