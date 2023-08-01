#pragma once

namespace MarsEngine
{
    class Radian;

    /** Wrapper class which indicates a given angle value is in Degrees.
    */
    class Degree
    {
    public:
        explicit Degree(float d = 0.0f) : m_degree(d) {}
        explicit Degree(Radian const& r);

        Degree& operator=(Degree const& d) = default;
        Degree& operator=(float f)
        {
            m_degree = f;
            return *this;
        }
        Degree& operator=(Radian const& r);

        explicit operator float() const { return m_degree; }
        explicit operator Radian() const;
        float toAngleUnits() const;

        Degree const& operator+ () const { return *this; }
        Degree        operator+ (Degree const& d) const { return Degree(m_degree + d.m_degree); }
        Degree&       operator+=(Degree const& d)
        {
            m_degree += d.m_degree;
            return *this;
        }
        Degree  operator- () const { return Degree(-m_degree); }
        Degree  operator- (Degree const& d) const { return Degree(m_degree - d.m_degree); }
        Degree& operator-=(Degree const& d)
        {
            m_degree -= d.m_degree;
            return *this;
        }
        Degree  operator* (float f) const { return Degree(m_degree * f); }
        Degree  operator* (Degree const& f) const { return Degree(m_degree * f.m_degree); }
        Degree& operator*=(float f)
        {
            m_degree *= f;
            return *this;
        }
        Degree  operator/ (float f) const { return Degree(m_degree / f); }
        Degree& operator/=(float f)
        {
            m_degree /= f;
            return *this;
        }

        bool operator< (Degree const& d) const { return m_degree <  d.m_degree; }
        bool operator<=(Degree const& d) const { return m_degree <= d.m_degree; }
        bool operator==(Degree const& d) const { return m_degree == d.m_degree; }
        bool operator!=(Degree const& d) const { return m_degree != d.m_degree; }
        bool operator>=(Degree const& d) const { return m_degree >= d.m_degree; }
        bool operator> (Degree const& d) const { return m_degree >  d.m_degree; }

    private:
        float m_degree;
    };

} // namespace MarsEngine