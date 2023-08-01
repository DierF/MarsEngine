#pragma once

namespace MarsEngine
{
    class Degree;

    /** Wrapper class which indicates a given angle value is in Radians.
    */
    class Radian
    {
    public:
        explicit Radian(float r = 0) : m_radian(r) {}
        explicit Radian(Degree const& d);

        Radian& operator=(Radian const& other) = default;
        Radian& operator=(float f)
        {
            m_radian = f;
            return *this;
        }
        Radian& operator=(Degree const& d);

        explicit operator float() const { return m_radian; }

        explicit operator Degree() const;

        float toAngleUnits() const;

        Radian const& operator+ () const { return *this; }
        Radian        operator+ (Radian const& r) const { return Radian(m_radian + r.m_radian); }
        Radian&       operator+=(Radian const& r)
        {
            m_radian += r.m_radian;
            return *this;
        }
        Radian  operator- () const { return Radian(-m_radian); }
        Radian  operator- (Radian const& r) const { return Radian(m_radian - r.m_radian); }
        Radian& operator-=(Radian const& r)
        {
            m_radian -= r.m_radian;
            return *this;
        }
        Radian  operator* (float f) const { return Radian(m_radian * f); }
        Radian  operator* (Radian const& f) const { return Radian(m_radian * f.m_radian); }
        Radian& operator*=(float f)
        {
            m_radian *= f;
            return *this;
        }
        Radian  operator/ (float f) const { return Radian(m_radian / f); }
        Radian& operator/=(float f)
        {
            m_radian /= f;
            return *this;
        }

        bool operator< (Radian const& r) const { return m_radian <  r.m_radian; }
        bool operator<=(Radian const& r) const { return m_radian <= r.m_radian; }
        bool operator==(Radian const& r) const { return m_radian == r.m_radian; }
        bool operator!=(Radian const& r) const { return m_radian != r.m_radian; }
        bool operator>=(Radian const& r) const { return m_radian >= r.m_radian; }
        bool operator> (Radian const& r) const { return m_radian >  r.m_radian; }

    private:
        float m_radian;
    };
} // namespace MarsEngine