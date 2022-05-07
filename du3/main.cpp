#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
    InvalidDateException ( )
            : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
public:
    CDate (int year, int month, int day );
    CDate operator + ( int day ) const;
    CDate operator - ( int day ) const;
    double operator - ( const CDate & other ) const;
    CDate & operator ++ ( void );
    CDate & operator -- ( void );
    CDate operator -- ( int );
    CDate operator ++ ( int );
    bool operator == ( const CDate & other ) const;
    bool operator != ( const CDate & other ) const;
    bool operator >= ( const CDate & other ) const;
    bool operator <= ( const CDate & other ) const;
    bool operator < ( const CDate & other ) const;
    bool operator > ( const CDate & other ) const;
    friend ostream & operator << ( ostream & os, const CDate & date );
    friend istream & operator >> ( istream & is, CDate & date );

private:
    int m_Year;
    int m_Month;
    int m_Day;
};
    bool checkDate ( int year, int month, int day)
{
    if (year < 2000 || year > 2030)
        return false;
    else {
        switch (month) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if (day > 0 && day <= 31) {
                    return true;
                } else
                    return false;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if (day > 0 && day <= 30) {
                   return true;
                } else
                    return false;
                break;
            case 2:
                if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
                    if (day > 0 && day <= 29) {
                        return true;
                    } else
                        return false;
                } else {
                    if (day > 0 && day <= 28) {
                       return true;
                    } else
                        return false;
                }
                break;
            default:
                return false;
        }
    }
}

CDate::CDate ( int year, int month, int day ) {

    if (year < 2000 || year > 2030)
        throw InvalidDateException();
    else {
        switch (month) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if (day > 0 && day <= 31) {
                    m_Day = day;
                    m_Year = year;
                    m_Month = month;
                } else
                    throw InvalidDateException();
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if (day > 0 && day <= 30) {
                    m_Day = day;
                    m_Year = year;
                    m_Month = month;
                } else
                    throw InvalidDateException();
                break;
            case 2:
                if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
                    if (day > 0 && day <= 29) {
                        m_Day = day;
                        m_Year = year;
                        m_Month = month;
                    } else
                        throw InvalidDateException();
                } else {
                    if (day > 0 && day <= 28) {
                        m_Day = day;
                        m_Year = year;
                        m_Month = month;
                    } else
                        throw InvalidDateException();
                }
                break;
            default:
                throw InvalidDateException();
        }
    }
}
CDate CDate::operator + ( int day ) const
{
        tm date = tm ();

        date . tm_year = m_Year - 1900;
        date . tm_mon = m_Month - 1;
        date . tm_mday = m_Day + day;

        mktime (& date);

        CDate newDate ( date . tm_year + 1900, date . tm_mon + 1, date . tm_mday );

        return newDate;
}
CDate CDate::operator - ( int day ) const
{
    tm date = tm ();

    date . tm_year = m_Year - 1900;
    date . tm_mon = m_Month - 1;
    date . tm_mday = m_Day - day;

    mktime (& date);

    CDate newDate ( date . tm_year + 1900, date . tm_mon + 1, date . tm_mday );

    return newDate;
}
double CDate::operator - ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return  abs ( ( ( difftime( mktime ( &date1 ), mktime ( &date2 ) ) )  / ( 3600 * 24 ) ) );
}
CDate & CDate:: operator -- ( void )
{
    tm date = tm ();

    date . tm_year = m_Year - 1900;
    date . tm_mon = m_Month - 1;
    date . tm_mday = m_Day - 1;

    mktime (& date);

    m_Month = date . tm_mon + 1;
    m_Year = date . tm_year + 1900;
    m_Day = date . tm_mday;

    return * this;
}
CDate & CDate:: operator ++ ( void )
{
    tm date = tm ();

    date . tm_year = m_Year - 1900;
    date . tm_mon = m_Month - 1;
    date . tm_mday = m_Day + 1;

    mktime (& date);

    m_Month = date . tm_mon + 1;
    m_Year = date . tm_year + 1900;
    m_Day = date . tm_mday;

    return * this;
}
CDate CDate::operator ++ ( int )
{
    CDate newOne ( m_Year, m_Month, m_Day );

    tm date = tm ();

    date . tm_year = m_Year - 1900;
    date . tm_mon = m_Month - 1;
    date . tm_mday = m_Day + 1;

    mktime ( & date );

    m_Month = date . tm_mon + 1;
    m_Year = date . tm_year + 1900;
    m_Day = date . tm_mday;

    return newOne;
}
CDate CDate::operator -- ( int )
{
    CDate newOne ( m_Year, m_Month, m_Day );

    tm date = tm ();

    date . tm_year = m_Year - 1900;
    date . tm_mon = m_Month - 1;
    date . tm_mday = m_Day - 1;

    mktime ( & date );

    m_Month = date . tm_mon + 1;
    m_Year = date . tm_year + 1900;
    m_Day = date . tm_mday;

    return newOne;
}
bool CDate::operator == ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return difftime( mktime ( &date1 ), mktime ( &date2 ) ) == 0;
}
bool CDate::operator != ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return  difftime( mktime ( &date1 ), mktime ( &date2 ) ) != 0 ;
}
bool CDate::operator >= ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return  difftime( mktime ( &date1 ), mktime ( &date2 ) ) >= 0 ;
}
bool CDate::operator <= ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return  difftime( mktime ( &date1 ), mktime ( &date2 ) ) <= 0 ;
}
bool CDate::operator < ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other. m_Day;

    return  difftime( mktime ( &date1 ), mktime ( &date2 ) ) < 0 ;
}
bool CDate::operator > ( const CDate & other ) const
{
    tm date1 = tm ();

    date1 . tm_year = m_Year - 1900;
    date1 . tm_mon = m_Month - 1;
    date1 . tm_mday = m_Day;

    tm date2 = tm ();

    date2 . tm_year = other . m_Year - 1900;
    date2 . tm_mon = other . m_Month - 1;
    date2 . tm_mday = other . m_Day;

    return  difftime( mktime ( &date1 ), mktime ( &date2 ) ) > 0 ;
}
ostream & operator << ( ostream & os, const CDate & date )
{
    os << date . m_Year << '-';
    if ( date . m_Month / 10 == 0 )
        os << '0';
    os << date . m_Month << '-';
    if ( date . m_Day / 10 == 0 )
        os << '0';
    os << date . m_Day;

    return os;
}
istream & operator >> ( istream & is, CDate & date)
{
    char c1, c2;
    int y, m, d;

    if ( !( is >> y >> c1 >> m >> c2 >> d )
         || c1 != '-'
         || c2 != '-'
         || !checkDate( y, m, d ) )
    {
        is . setstate ( ios::failbit );
        return is;
    }

    date . m_Year = y;
    date . m_Month = m;
    date . m_Day = d;

    return is;
}


#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;
    istringstream iss;

    CDate a ( 2000, 1, 2 );
    CDate b ( 2010, 2, 3 );
    CDate c ( 2004, 2, 10 );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2000-01-02" );
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2010-02-03" );
    oss . str ("");
    oss << c;
    assert ( oss . str () == "2004-02-10" );
    a = a + 1500;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2004-02-10" );
    b = b - 2000;
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2004-08-13" );
    assert ( b - a == 185 );
    assert ( ( b == a ) == false );
    assert ( ( b != a ) == true );
    assert ( ( b <= a ) == false );
    assert ( ( b < a ) == false );
    assert ( ( b >= a ) == true );
    assert ( ( b > a ) == true );
    assert ( ( c == a ) == true );
    assert ( ( c != a ) == false );
    assert ( ( c <= a ) == true );
    assert ( ( c < a ) == false );
    assert ( ( c >= a ) == true );
    assert ( ( c > a ) == false );
    a = ++c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-11" );
    a = --c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-10" );
    a = c++;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-11" );
    a = c--;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-10" );
    iss . clear ();
    iss . str ( "2015-09-03" );
    assert ( ( iss >> a ) );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-09-03" );
    a = a + 70;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-11-12" );

    CDate d ( 2000, 1, 1 );
    try
    {
        CDate e ( 2000, 32, 1 );
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( ... )
    {
    }
    iss . clear ();
    iss . str ( "2000-12-33" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-11-31" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-02-29" );
    assert ( ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
    iss . clear ();
    iss . str ( "2001-02-29" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );

    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------
    CDate f ( 2000, 5, 12 );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    oss . str ("");
    oss << date_format ( "%Y/%m/%d" ) << f;
    assert ( oss . str () == "2000/05/12" );
    oss . str ("");
    oss << date_format ( "%d.%m.%Y" ) << f;
    assert ( oss . str () == "12.05.2000" );
    oss . str ("");
    oss << date_format ( "%m/%d/%Y" ) << f;
    assert ( oss . str () == "05/12/2000" );
    oss . str ("");
    oss << date_format ( "%Y%m%d" ) << f;
    assert ( oss . str () == "20000512" );
    oss . str ("");
    oss << date_format ( "hello kitty" ) << f;
    assert ( oss . str () == "hello kitty" );
    oss . str ("");
    oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
    assert ( oss . str () == "121212121212050505200020002000%%%%%" );
    oss . str ("");
    oss << date_format ( "%Y-%m-%d" ) << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-1" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-1-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-001-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-02" );
    assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2001-01-02" );
    iss . clear ();
    iss . str ( "05.06.2003" );
    assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2003-06-05" );
    iss . clear ();
    iss . str ( "07/08/2004" );
    assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2004-07-08" );
    iss . clear ();
    iss . str ( "2002*03*04" );
    assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2002-03-04" );
    iss . clear ();
    iss . str ( "C++09format10PA22006rulez" );
    assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2006-09-10" );
    iss . clear ();
    iss . str ( "%12%13%2010%" );
    assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2010-12-13" );

    CDate g ( 2000, 6, 8 );
    iss . clear ();
    iss . str ( "2001-11-33" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "29.02.2003" );
    assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "14/02/2004" );
    assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2002-03" );
    assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "hello kitty" );
    assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2005-07-12-07" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "20000101" );
    assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-01-01" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
