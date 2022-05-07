#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */


template <class T>
class Vector {

    T * m_Data;
    uint32_t m_Size;

public:
    uint32_t m_Current;

    Vector(uint32_t size = 10)
    {
        m_Size = size;
        m_Current = 0;
        m_Data = new T [size];
    }

    Vector(const Vector<T> &src)
    {
        m_Size = src . m_Size;
        m_Current = src . m_Current;
        m_Data = new T [ m_Size ];
        for ( uint32_t i = 0; i < m_Current; ++ i )
            m_Data [i] = src . m_Data [i];
    }

    Vector<T> & operator= (const Vector<T> &src)
    {
        if ( this != &src )
        {
            delete [] m_Data;
            m_Size = src . m_Size;
            m_Current = src . m_Current;
            m_Data = new T [m_Size];
            for (uint32_t i = 0; i < m_Current; ++ i )
                m_Data [i] = src . m_Data [i];
        }
        return *this;
    }

    ~Vector()
    {
        delete [] m_Data;
        m_Data = nullptr;
        m_Size = 0;
        m_Current = 0;
    }
    uint32_t size () const { return m_Size; }

    const T & operator [] ( uint32_t idx ) const
    {
        if ( idx < 0 || idx >= m_Size ) throw "Bad index!";
        return m_Data [idx];
    }
    T & operator [] ( uint32_t idx )
    {
        if ( idx < 0 || idx >= m_Size ) throw "Bad index!";
        return m_Data [idx];
    }
    void push ( T src )
    {
        if ( m_Current + 1 >= m_Size )
        {
            m_Size *= 1.5;
            T * temp = new T [m_Size];
            for ( uint32_t i = 0; i < m_Current; ++ i )
                temp [i] = m_Data [i];
            delete [] m_Data;
            m_Data = temp;
        }
        m_Data [m_Current++] = src;
    }
    void pop ()
    {
        m_Current--;
    }
};




class CFile
{
public:
    CFile                         ( void )
    {
    }

    ~CFile                         ( void )
    {
    }

    CFile ( const CFile & src )
    {
        for (uint32_t i = 0; i <= src . versions . m_Current; i++)
        {
            versions . push (src.versions[i]);
        }
        versions . m_Current --;
    }

    CFile & operator = ( const CFile & src )
    {
        if ( this != &src )
        {
            for (uint32_t i = 0; i < src . versions .m_Current; i++)
            {
            versions. push (src . versions[i]);

            }
        }
        return *this;
    }

    bool                     seek                          ( uint32_t          offset )
    {
        if ( offset < 0 || offset > versions [versions.m_Current] . oneVersion . m_Current )
            return false;
        versions[versions . m_Current] . position = offset;
        return true;
    }
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes )
    {
        uint32_t i = 0;
        while ( versions [versions . m_Current] . position  < versions [versions . m_Current] . oneVersion . m_Current && i < bytes )
        {
            dst [i++] = versions [versions . m_Current] . oneVersion [ versions[versions . m_Current] . position ++];
        }
        return i;
    }

    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes )
    {
        uint32_t i = 0;
        while ( i < bytes && versions [versions . m_Current] . position < versions [versions.m_Current] . oneVersion . m_Current )
            versions[versions.m_Current] . oneVersion[versions[versions.m_Current].position++] = src[i++];

        for ( ; i < bytes; i ++)
        {
            versions[versions.m_Current] . oneVersion .push( src [i]);
            versions[versions.m_Current] .position++;
        }

        return i;
    }
    void                     truncate                      ( void )
    {
        versions [versions.m_Current] . oneVersion .m_Current = versions [versions.m_Current] . position;
    }
    uint32_t                 fileSize                      ( void ) const
    {
        return  versions [versions.m_Current] . oneVersion . m_Current;
    }
    void                     addVersion                    ( void )
    {
        Version temp (versions[versions.m_Current++]);
        versions . push (temp);
        versions.m_Current--;

    }
    bool                     undoVersion                   ( void )
    {
        if ( versions .m_Current <= 0 ) return false;
        versions . pop();
        return true;
    }
private:
    struct Version
    {
        Vector <uint8_t> oneVersion;
        uint32_t position;

        Version ()
        {
            position = 0;
        }
        Version ( const Version & src )
        {
            position = src . position;
            oneVersion = src . oneVersion;
        }
        Version & operator = ( const Version & src )
        {
            if (this != &src)
            {
                position = src . position;
                oneVersion = src . oneVersion;
            }
            return *this;
        }
        ~Version(){}

    };

    Vector <Version> versions;
};




















#ifndef __PROGTEST__
bool               writeTest                               ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          wrLen )
{
    return x . write ( data . begin (), data . size () ) == wrLen;
}

bool               readTest                                ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          rdLen )
{
    uint8_t  tmp[100];
    uint32_t idx = 0;

    if ( x . read ( tmp, rdLen ) != data . size ())
        return false;
    for ( auto v : data )
        if ( tmp[idx++] != v )
            return false;
    return true;
}

int main ( void )
{
    CFile f0;

    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    assert ( f0 . fileSize () == 3 );
    assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 2 ));
    assert ( writeTest ( f0, { 5, 4 }, 2 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 1 ));
    assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
    assert ( f0 . seek ( 3 ));
    f0 . addVersion();
    assert ( f0 . seek ( 6 ));
    assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
    f0 . addVersion();
    assert ( f0 . seek ( 5 ));
    CFile f1 ( f0 );
    f0 . truncate ();
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
    assert ( !f0 . seek ( 100 ));
    assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
    assert ( f1 . seek ( 0 ));
    assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
    assert ( f1 . undoVersion () );
    assert ( f1 . undoVersion () );
    assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
    assert ( !f1 . undoVersion () );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
