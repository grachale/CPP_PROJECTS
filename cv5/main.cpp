#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    // constructor, destructor
    CTime () {}
    CTime ( int a, int b, int c = 0 )
    {
        if (a > 23 || a < 0 ||
            b > 59 || b < 0 ||
            c > 59 || c < 0 )
            throw std::invalid_argument("Invalid arguments.");
        m_Hour = a;
        m_Minute = b;
        m_Second = c;
    }
    CTime ( const CTime & src )
            : m_Hour ( src . m_Hour ),
              m_Minute ( src . m_Minute ),
              m_Second ( src . m_Second )
    {
    }
    ~CTime ()
    {
    }
    // arithmetic operators

    CTime operator + (const int x ) const
    {
        CTime newDate = CTime ( * this );
        int special, special2 = 24 * 3600;;
        if ( x < 0 )
        {
            special = ( m_Second + 60 * m_Minute + 3600 * m_Hour ) - abs ( x );
            special %= special2;
            if ( special < 0 )
            {
                special = special2 - abs ( special );
                newDate . m_Hour = special / 3600;
                special %= 3600;
                newDate . m_Minute = special / 60;
                special %= 60;
                newDate . m_Second = special;

                return newDate;
            }
        } else {
        special = ( x % special2 ) + m_Second + m_Minute * 60 + 3600 * m_Hour;
        special %= special2;
        }

        newDate . m_Hour = special / 3600;
        special %= 3600;
        newDate . m_Minute = special / 60;
        special %= 60;
        newDate . m_Second = special;

        return newDate;

    }

    CTime & operator += ( const int x )
    {
        int special, special2 = 24 * 3600;
        if ( x < 0 ) {
            special = ( m_Second + 60 * m_Minute + 3600 * m_Hour ) - abs ( x );
            special %= special2;

            if ( special < 0 )
            {
                special = special2 - abs ( special );
                m_Hour = special / 3600;
                special %= 3600;
                m_Minute = special / 60;
                special %= 60;
                m_Second = special;

                return * this;
            }

        } else {
        special = ( x % special2 ) + m_Second + m_Minute * 60 + 3600 * m_Hour;
        special %= special2;
        }

        m_Hour = special / 3600;
        special %= 3600;
        m_Minute = special / 60;
        special %= 60;
        m_Second = special;

        return * this;
    }

    CTime & operator -= ( const int x ) {
        int special, special2 = 24 * 3600;
        if ( x < 0 ) {
            special = ( abs ( x ) % special2 ) + m_Second + m_Minute * 60 + 3600 * m_Hour;
            special %= special2;
        } else {
            special = (m_Second + 60 * m_Minute + 3600 * m_Hour) - x;
            special %= special2;

        if (special < 0) {
            special = special2 - abs(special);
            m_Hour = special / 3600;
            special %= 3600;
            m_Minute = special / 60;
            special %= 60;
            m_Second = special;

            return *this;
        }
    }
        m_Hour = special / 3600;
        special %= 3600;
        m_Minute = special / 60;
        special %= 60;
        m_Second = special;

        return * this;
    }

    CTime operator - ( const int x ) const
    {
        CTime newDate = CTime ( * this );
        int special, special2 = 24 * 3600;
        if ( x < 0 ) {
            special = ( abs ( x ) % special2 ) + m_Second + m_Minute * 60 + 3600 * m_Hour;
            special %= special2;
        } else {
        special = ( m_Second + 60 * m_Minute + 3600 * m_Hour ) - x;
        special %= special2;
        if ( special < 0 )
        {
            special = special2 - abs ( special );
            newDate . m_Hour = special / 3600;
            special %= 3600;
            newDate . m_Minute = special / 60;
            special %= 60;
            newDate . m_Second = special;

            return newDate;
        }
        }

        newDate . m_Hour = special / 3600;
        special %= 3600;
        newDate . m_Minute = special / 60;
        special %= 60;
        newDate . m_Second = special;

        return newDate;
    }

    int operator - ( const CTime & other ) const
    {
        return  abs ( (m_Second + 60 * m_Minute + 3600 * m_Hour ) - ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour ) );
    }

    CTime & operator++ ( void ) {
        int seconds = m_Second + 1;
        int minutes = ( seconds / 60 ) + m_Minute;
        int hours = ( minutes / 60 ) + m_Hour;

        m_Second = seconds % 60;
        m_Minute = minutes % 60;
        m_Hour = hours % 24;
        return * this;
    }
    CTime & operator-- ( void )
    {
        int special = ( m_Second + 60 * m_Minute + 3600 * m_Hour ) - 1;

        if ( special < 0 ) {
            m_Second = 23;
            m_Minute = 59;
            m_Hour   = 59;
            return * this;
        }
        m_Hour = special / 3600;
        special %= 3600;
        m_Minute = special / 60;
        special %= 60;
        m_Second = special;
        return * this;
    }
    CTime operator-- ( int )
    {
        CTime newDate = CTime ( * this );
        int special = ( m_Second + 60 * m_Minute + 3600 * m_Hour ) - 1;

        if ( special < 0 ) {
            this -> m_Second = 23;
            this -> m_Minute = 59;
            this -> m_Hour   = 59;
            return newDate;
        }
        this -> m_Hour = special / 3600;
        special %= 3600;
        this -> m_Minute = special / 60;
        special %= 60;
        this -> m_Second = special;

        return newDate;
    }
    CTime operator++ ( int )
    {
        CTime newDate = CTime ( * this );
        int seconds = m_Second + 1;
        int minutes = ( seconds / 60 ) + m_Minute;
        int hours = ( minutes / 60 ) + m_Hour;

        m_Second = seconds % 60;
        m_Minute = minutes % 60;
        m_Hour = hours % 24;

        return newDate;
    }

    bool operator < ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) < ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }
    bool operator > ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) > ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }
    bool operator >= ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) >= ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }
    bool operator <= ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) <= ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }
    bool operator == ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) == ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }
    bool operator != ( const CTime & other) const
    {
        return  (m_Second + 60 * m_Minute + 3600 * m_Hour ) != ( other . m_Second + 60 * other . m_Minute + 3600 * other . m_Hour );
    }

    friend ostream & operator << ( ostream & os, const CTime & our )
    {
        if ( ( our . m_Hour / 10 ) == 0 )
            os << ' ';

        os  << our . m_Hour << ':';

        if ( our . m_Minute == 0 )
            os << '0';
        else if ( (our . m_Minute / 10) == 0 )
            os << ' ';

        os << our . m_Minute << ':';

        if ( our . m_Second == 0 )
            os << '0';
        else if ( ( our . m_Second / 10) == 0 )
            os << ' ';

        os << our . m_Second;
        return os;
    }


    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour
               && time.m_Minute == minute
               && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a ( 12, 30 );
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b ( 13, 30 );
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
