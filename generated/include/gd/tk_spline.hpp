#pragma once

class tk_spline {
public:
    std::vector<double> m_x;
    std::vector<double> m_y;
    std::vector<double> m_b;
    std::vector<double> m_c;
    std::vector<double> m_d;
    double m_c0;
    int m_type;
    int m_left;
    int m_right;
    double m_leftValue;
    double m_rightValue;
    bool m_madeMonotonic;

};
