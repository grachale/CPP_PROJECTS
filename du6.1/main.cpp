#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */




class CDataType
{
public:
    CDataType () {};
    virtual ~CDataType() {};
    virtual size_t getSize ( void ) const = 0;
    virtual void Print ( ostream & os ) const = 0;
    virtual bool operator == ( const CDataType & src) const = 0;
    virtual bool operator != ( const CDataType & src) const = 0;
    virtual CDataType * Clone ( void ) const = 0;
};


class CDataTypeInt : public CDataType
{
public:
    CDataTypeInt ( ) = default;
    ~CDataTypeInt()
    {}

    virtual CDataType * Clone ( void ) const override
    {
        return new CDataTypeInt ( * this );
    }

    virtual size_t getSize () const override
    {
        return 4;
    }
    virtual bool operator == (const CDataType & src ) const override
    {
        return typeid(*this) == typeid(src);
    }
    virtual bool operator != (const CDataType & src ) const override
    {
        return typeid(*this) != typeid(src);
    }
    virtual void Print  ( ostream & os ) const override
    {
        os << "int";
    }
};



class CDataTypeDouble : public CDataType
{
public:
    CDataTypeDouble ( ) = default;
    ~CDataTypeDouble ( )
    {}
    virtual CDataType * Clone ( void ) const override
    {
        return new CDataTypeDouble ( * this );
    }
    virtual size_t getSize () const override
    {
        return 8;
    }
    virtual bool operator == (const CDataType & src ) const override
    {
        return typeid(*this) == typeid(src);
    }
    virtual bool operator != (const CDataType & src ) const override {
        return typeid(*this) != typeid(src);
    }
    virtual void Print ( ostream & os ) const override
    {
        os << "double";
    }
};



class CDataTypeEnum : public CDataType
{
public:
    CDataTypeEnum()
    {
    }
    virtual ~CDataTypeEnum()
    {}

    CDataTypeEnum ( const CDataTypeEnum & src )
    {
        elements = src . elements;
    }

    virtual CDataType * Clone ( void ) const override
    {
        return new CDataTypeEnum ( * this );
    }

    virtual size_t getSize () const override
    {
        return 4;
    }

    virtual bool operator == ( const CDataType & src ) const override
    {
        return typeid(*this) == typeid(src) && dynamic_cast<const CDataTypeEnum &> ( src ) . elements == elements;
    }

    virtual bool operator != (const CDataType & src ) const override
    {
        return typeid(*this) != typeid(src) || dynamic_cast<const CDataTypeEnum &> ( src ) . elements != elements;
    }

    virtual void Print ( ostream & os ) const override
    {
        os << "enum\n" << "{\n";
        for ( long unsigned int i = 0; i < elements . size() - 1; ++ i)
            os << elements [i] << ",\n";
        os << elements[ elements . size () - 1] << "\n} ";
    }
    CDataTypeEnum & add ( const string & newElement )
    {
        if ( count ( elements . begin (), elements . end(), newElement ) )
            throw invalid_argument("Duplicate enum value: " + newElement);
        elements . push_back ( newElement );
        return *this;
    }
private:
    vector<string> elements;
};



class CDataTypeStruct : public CDataType
{
public:
    CDataTypeStruct()
            : size ( 0 )
    {}
    CDataTypeStruct ( const CDataTypeStruct & src )
    {
        for ( const string & name : src . order )
            elements . insert ( {name, src .elements . find ( name ) -> second -> Clone () } );
        order = src . order;
        size = src . size;
    }

    virtual ~CDataTypeStruct () override
    {
        for ( size_t i = 0; i < order .size(); ++i )
            delete elements . find ( order [i] ) -> second;
    }
    virtual CDataType * Clone ( void ) const override
    {
        return new CDataTypeStruct ( * this );
    }
    virtual size_t getSize () const override
    {
        return size;
    }

    virtual bool operator == ( const CDataType & src ) const override
    {
        if ( typeid(*this) == typeid(src) ) {
            auto it = dynamic_cast<const CDataTypeStruct &> ( src );
            if ( it . order . size() == order . size() )
            {
                for ( size_t i = 0; i < order . size (); ++i )
                {
                    if ( typeid( elements . find ( order[i] ) -> second) !=
                         typeid( it. elements . find ( it . order[i] ) -> second ) || *elements . find ( order[i] ) -> second !=
                                                                                      *it. elements . find ( it . order[i] ) -> second  )
                        return false;
                }
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    virtual bool operator != (const CDataType & src ) const override
    {
        if ( typeid(*this) == typeid(src) ) {
            auto it = dynamic_cast<const CDataTypeStruct &> ( src );
            if ( it . order . size() == order . size() )
            {
                for ( size_t i = 0; i < order . size (); ++i )
                {
                    if ( typeid( elements . find ( order[i] ) -> second) !=
                         typeid( it. elements . find ( it . order[i] ) -> second ) ||
                         *elements . find ( order[i] ) -> second !=
                         *it. elements . find ( it . order[i] ) -> second  )
                        return true;
                }
                return false;
            }
            else {
                return true;
            }
        }
        else {
            return true;
        }
    }

    virtual void Print (  ostream & os ) const override
    {
        os << "struct\n{\n";
        for ( size_t i = 0; i < order . size (); ++i )
        {

            auto it = elements . find ( order [i] );
            it-> second -> Print ( os );
            os << order [i] << ";\n";
        }
        os << '}';
    }
    CDataTypeStruct & addField ( string name, const CDataType & x )
    {
        if ( elements . find ( name ) != elements . end () )
            throw invalid_argument( "Duplicate field: " + name );
        elements . insert ( { name,  x . Clone() } );
        order .push_back ( name );
        size += x . getSize();
        return *this;
    }

    CDataType & field ( string name ) const
    {
        auto pos = elements . find ( name );
        if ( pos == elements . end () )
            throw invalid_argument( "Unknown field: " + name );
        return * pos -> second;
    }

private:
    vector<string> order;
    unordered_map< string, CDataType* > elements;
    size_t size;
};

ostream & operator << ( ostream & os, const CDataType & x )
{
    x . Print( os );
    return os;
}


class CDataTypeArray : public CDataType
{
    CDataTypeArray ( size_t size, const CDataType & element )
    {
        m_PtrElement = element . Clone();
        m_Size = size;
    };
    virtual ~CDataTypeArray() override
    {
        delete m_PtrElement;
    }
    virtual size_t getSize ( void ) const override
    {
        return m_PtrElement ->getSize() * m_Size;
    }

    virtual void Print ( ostream & os ) const = 0;
    virtual bool operator == ( const CDataType & src) const = 0;
    virtual bool operator != ( const CDataType & src) const = 0;
    virtual CDataType * Clone ( void ) const = 0;
protected:
    CDataType * m_PtrElement;
    size_t m_Size;

};
class CDataTypePtr
{
    // todo
};
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
    for ( long unsigned int j = 0, i = 0; j < b . size () && i < a . size (); ++j, ++i )
    {
        while ( isspace ( a[i] ) && i < a . size () )
            ++i;
        while ( isspace ( b[j] ) && j < b . size () )
            ++j;
        if ( a[i] != b[j] )
            return false;
    }
    return true;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
    ostringstream oss;
    oss << x;
    return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{

    CDataTypeStruct  a = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct b = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "READY" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct c =  CDataTypeStruct () .
            addField ( "m_First", CDataTypeInt () ) .
            addField ( "m_Second", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Third", CDataTypeDouble () );

    CDataTypeStruct  d = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeInt () );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( c, "struct\n"
                                  "{\n"
                                  "  int m_First;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Second;\n"
                                  "  double m_Third;\n"
                                  "}") );

    assert ( whitespaceMatch ( d, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  int m_Ratio;\n"
                                  "}") );

    assert ( a != b );
    assert ( a == c );
    assert ( a != d );
    assert ( a != CDataTypeInt() );
    assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
                                                         "{\n"
                                                         "  NEW,\n"
                                                         "  FIXED,\n"
                                                         "  BROKEN,\n"
                                                         "  DEAD\n"
                                                         "}") );

    b . addField ( "m_Other", a );

    a . addField ( "m_Sum", CDataTypeInt ());

    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  int m_Sum;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      DEAD\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "  } m_Other;\n"
                                  "}") );

    assert ( whitespaceMatch ( b . field ( "m_Other" ), "struct\n"
                                                        "{\n"
                                                        "  int m_Length;\n"
                                                        "  enum\n"
                                                        "  {\n"
                                                        "    NEW,\n"
                                                        "    FIXED,\n"
                                                        "    BROKEN,\n"
                                                        "    DEAD\n"
                                                        "  } m_Status;\n"
                                                        "  double m_Ratio;\n"
                                                        "}") );

    assert ( whitespaceMatch ( b . field ( "m_Other" ) . field ( "m_Status" ), "enum\n"
                                                                               "{\n"
                                                                               "  NEW,\n"
                                                                               "  FIXED,\n"
                                                                               "  BROKEN,\n"
                                                                               "  DEAD\n"
                                                                               "}") );

    assert ( a . getSize () == 20 );
    assert ( b . getSize () == 32 );
    b . addField ( "m_Other1", b );
    b . addField ( "m_Other2", b );
    b . addField ( "m_Other3", b );
    assert ( b . field ( "m_Other3" ) . field ( "m_Other2" ) . field ( "m_Other1" ) == b . field ( "m_Other1" ) );

    assert ( b . getSize () == 256);

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      DEAD\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "  } m_Other;\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      READY\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        DEAD\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "    } m_Other;\n"
                                  "  } m_Other1;\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      READY\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        DEAD\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "    } m_Other;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        READY\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "      struct\n"
                                  "      {\n"
                                  "        int m_Length;\n"
                                  "        enum\n"
                                  "        {\n"
                                  "          NEW,\n"
                                  "          FIXED,\n"
                                  "          BROKEN,\n"
                                  "          DEAD\n"
                                  "        } m_Status;\n"
                                  "        double m_Ratio;\n"
                                  "      } m_Other;\n"
                                  "    } m_Other1;\n"
                                  "  } m_Other2;\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      READY\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        DEAD\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "    } m_Other;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        READY\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "      struct\n"
                                  "      {\n"
                                  "        int m_Length;\n"
                                  "        enum\n"
                                  "        {\n"
                                  "          NEW,\n"
                                  "          FIXED,\n"
                                  "          BROKEN,\n"
                                  "          DEAD\n"
                                  "        } m_Status;\n"
                                  "        double m_Ratio;\n"
                                  "      } m_Other;\n"
                                  "    } m_Other1;\n"
                                  "    struct\n"
                                  "    {\n"
                                  "      int m_Length;\n"
                                  "      enum\n"
                                  "      {\n"
                                  "        NEW,\n"
                                  "        FIXED,\n"
                                  "        BROKEN,\n"
                                  "        READY\n"
                                  "      } m_Status;\n"
                                  "      double m_Ratio;\n"
                                  "      struct\n"
                                  "      {\n"
                                  "        int m_Length;\n"
                                  "        enum\n"
                                  "        {\n"
                                  "          NEW,\n"
                                  "          FIXED,\n"
                                  "          BROKEN,\n"
                                  "          DEAD\n"
                                  "        } m_Status;\n"
                                  "        double m_Ratio;\n"
                                  "      } m_Other;\n"
                                  "      struct\n"
                                  "      {\n"
                                  "        int m_Length;\n"
                                  "        enum\n"
                                  "        {\n"
                                  "          NEW,\n"
                                  "          FIXED,\n"
                                  "          BROKEN,\n"
                                  "          READY\n"
                                  "        } m_Status;\n"
                                  "        double m_Ratio;\n"
                                  "        struct\n"
                                  "        {\n"
                                  "          int m_Length;\n"
                                  "          enum\n"
                                  "          {\n"
                                  "            NEW,\n"
                                  "            FIXED,\n"
                                  "            BROKEN,\n"
                                  "            DEAD\n"
                                  "          } m_Status;\n"
                                  "          double m_Ratio;\n"
                                  "        } m_Other;\n"
                                  "      } m_Other1;\n"
                                  "    } m_Other2;\n"
                                  "  } m_Other3;\n"
                                  "}" ) );

    try
    {
        a . addField ( "m_Status", CDataTypeInt () );
        assert ( "addField: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate field: m_Status"sv );
    }

    try
    {
        cout << a . field ( "m_Fail" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Unknown field: m_Fail"sv );
    }

    try
    {
        cout << a . field ( "m_Length" ) . field ( "m_Foo" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int" ));
    }

    try
    {
        cout << a . field ( "m_Status" ) . field ( "m_Foo" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: enum\n"
                                                "{\n"
                                                "  NEW,\n"
                                                "  FIXED,\n"
                                                "  BROKEN,\n"
                                                "  DEAD\n"
                                                "}" ));
    }

    try
    {
        CDataTypeEnum en;
        en . add ( "FIRST" ) .
                add ( "SECOND" ) .
                add ( "FIRST" );
        assert ( "add: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate enum value: FIRST"sv );
    }

    CDataTypeArray ar1 ( 10, CDataTypeInt () );
    assert ( whitespaceMatch ( ar1, "int[10]") );
    assert ( whitespaceMatch ( ar1 . element (), "int") );
    CDataTypeArray ar2 ( 11, ar1 );
    assert ( whitespaceMatch ( ar2, "int[11][10]") );
    assert ( whitespaceMatch ( ar2 . element (), "int[10]") );
    assert ( whitespaceMatch ( ar2 . element () . element (), "int") );
    CDataTypeArray ar3 ( 10, CDataTypeArray ( 20, CDataTypePtr ( CDataTypeInt () ) ) );
    assert ( whitespaceMatch ( ar3, "int*[10][20]") );
    assert ( whitespaceMatch ( ar3 . element (), "int*[20]") );
    assert ( whitespaceMatch ( ar3 . element () . element (), "int*") );
    assert ( whitespaceMatch ( ar3 . element () . element () . element (), "int") );
    CDataTypePtr ar4  ( ar1 . element () );
    assert ( whitespaceMatch ( ar4, "int*") );
    assert ( whitespaceMatch ( ar4 . element (), "int") );
    CDataTypePtr ar5  ( b . field ( "m_Status" ) );
    assert ( whitespaceMatch ( ar5, "enum\n"
                                    "{\n"
                                    "  NEW,\n"
                                    "  FIXED,\n"
                                    "  BROKEN,\n"
                                    "  READY\n"
                                    "}*") );
    assert ( whitespaceMatch ( ar5 . element (), "enum\n"
                                                 "{\n"
                                                 "  NEW,\n"
                                                 "  FIXED,\n"
                                                 "  BROKEN,\n"
                                                 "  READY\n"
                                                 "}") );
    CDataTypePtr ar6 ( ar3 . element () . element () );
    assert ( whitespaceMatch ( ar6, "int**") );
    assert ( whitespaceMatch ( ar6 . element (), "int*") );
    assert ( whitespaceMatch ( ar6 . element () . element (), "int") );
    CDataTypePtr ar7 ( CDataTypeArray ( 50, ar6 ) );
    assert ( whitespaceMatch ( ar7, "int**(*)[50]") );
    assert ( whitespaceMatch ( ar7 . element (), "int**[50]") );
    assert ( whitespaceMatch ( ar7 . element () . element (), "int**") );
    assert ( whitespaceMatch ( ar7 . element () . element () . element (), "int*") );
    assert ( whitespaceMatch ( ar7 . element () . element () . element () . element (), "int") );
    CDataTypeArray ar8 ( 25, ar7 );
    assert ( whitespaceMatch ( ar8, "int**(*[25])[50]") );
    CDataTypePtr ar9 ( ar8 );
    assert ( whitespaceMatch ( ar9, "int**(*(*)[25])[50]") );
    a . addField ( "m_Ar1", ar1 ) .
            addField ( "m_Ar2", ar2 ) .
            addField ( "m_Ar3", ar3 ) .
            addField ( "m_Ar4", ar4 ) .
            addField ( "m_Ar5", ar5 ) .
            addField ( "m_Ar6", ar6 ) .
            addField ( "m_Ar7", ar7 ) .
            addField ( "m_Ar8", ar8 ) .
            addField ( "m_Ar9", ar9 );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  int m_Sum;\n"
                                  "  int m_Ar1[10];\n"
                                  "  int m_Ar2[11][10];\n"
                                  "  int* m_Ar3[10][20];\n"
                                  "  int* m_Ar4;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  }* m_Ar5;\n"
                                  "  int** m_Ar6;\n"
                                  "  int**(* m_Ar7)[50];\n"
                                  "  int**(* m_Ar8[25])[50];\n"
                                  "  int**(*(* m_Ar9)[25])[50];\n"
                                  "}") );
    a . addField ( "m_Madness", CDataTypeArray ( 40, CDataTypePtr ( a ) ) );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  int m_Sum;\n"
                                  "  int m_Ar1[10];\n"
                                  "  int m_Ar2[11][10];\n"
                                  "  int* m_Ar3[10][20];\n"
                                  "  int* m_Ar4;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  }* m_Ar5;\n"
                                  "  int** m_Ar6;\n"
                                  "  int**(* m_Ar7)[50];\n"
                                  "  int**(* m_Ar8[25])[50];\n"
                                  "  int**(*(* m_Ar9)[25])[50];\n"
                                  "  struct\n"
                                  "  {\n"
                                  "    int m_Length;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      DEAD\n"
                                  "    } m_Status;\n"
                                  "    double m_Ratio;\n"
                                  "    int m_Sum;\n"
                                  "    int m_Ar1[10];\n"
                                  "    int m_Ar2[11][10];\n"
                                  "    int* m_Ar3[10][20];\n"
                                  "    int* m_Ar4;\n"
                                  "    enum\n"
                                  "    {\n"
                                  "      NEW,\n"
                                  "      FIXED,\n"
                                  "      BROKEN,\n"
                                  "      READY\n"
                                  "    }* m_Ar5;\n"
                                  "    int** m_Ar6;\n"
                                  "    int**(* m_Ar7)[50];\n"
                                  "    int**(* m_Ar8[25])[50];\n"
                                  "    int**(*(* m_Ar9)[25])[50];\n"
                                  "  }* m_Madness[40];\n"
                                  "}") );
    assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) == a . field ( "m_Ar9" ));
    assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) != a . field ( "m_Ar8" ));
    try
    {
        cout << ar2 . field ( "m_Foo" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int[11][10]" ));
    }

    try
    {
        cout << c . element () << endl;
        assert ( "element: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( whitespaceMatch ( e . what (), "Cannot use element() for type: struct\n"
                                                "{\n"
                                                "  int m_First;\n"
                                                "  enum\n"
                                                "  {\n"
                                                "    NEW,\n"
                                                "    FIXED,\n"
                                                "    BROKEN,\n"
                                                "    DEAD\n"
                                                "  } m_Second;\n"
                                                "  double m_Third;\n"
                                                "}" ));
    }

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
