#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


int pow ( int base, int power )
{
    int result = 1;
    for ( int i = 0; i < power; i ++ )
        result *= base;
    return result;
}


class CNode
{
public:
    uint8_t data[4];
    CNode * left;
    CNode * right;

    CNode ( uint8_t * x = nullptr, CNode * left = nullptr, CNode * right = nullptr )
    : left ( left ),
      right ( right )
    {
        if ( x == nullptr ) {
            for ( size_t i = 0; i < 4; i ++ )
                data[i] = '\0';
        } else {
            for (size_t i = 0; i < 4; i ++ )
                data[i] = x[i];
        }
    }
};

class CTree
{
public:
    CTree ()
    {
        start = new CNode ();

    }
    ~CTree()
    {
        deleteTree ( start );
    }
    bool recursionCreation (CNode * node, const vector<int> & vect, int & readedSignes )
    {
        if ( node -> left != nullptr && node -> right != nullptr )
            return true;

        if ( 0 == vect[ readedSignes ] )
        {
            readedSignes++;
            if ( node -> left == nullptr )
            {
                CNode * newOne = new CNode ();
                node -> left = newOne;
                if ( !recursionCreation( node -> left, vect, readedSignes ) )
                    return false;
                return recursionCreation( node, vect, readedSignes );
            } else if ( node -> right == nullptr )
            {
                CNode * newOne = new CNode ();
                node -> right = newOne;
                if ( !recursionCreation( node -> right, vect, readedSignes ) )
                    return false;
                return recursionCreation( node, vect, readedSignes );
            } else {
            return false;
            }
        } else {
            readedSignes++;
            if ( node -> left == nullptr )
            {
                CNode * newOne = new CNode ();
                if ( !binaryToChar ( vect, readedSignes, newOne -> data ) )
                    return false;
                node -> left = newOne;
                return recursionCreation( node, vect, readedSignes);
            } else if ( node -> right == nullptr )
            {
                CNode * newOne = new CNode ( );
                if ( !binaryToChar ( vect, readedSignes, newOne -> data ) )
                    return false;
                node -> right = newOne;
                return true;
            } else {
            return false;
            }
        }

    }

    CNode * start;
private:
    void deleteTree ( CNode * node )
    {
        if ( node == nullptr ) return;

        deleteTree( node -> left);
        deleteTree ( node -> right );

        delete node;
    }
    bool binaryToChar ( const vector<int> & vect, int & readedSignes, uint8_t * sign ) {
        if (vect[readedSignes] == 0) {
            for (int j = 7; j >= 0; j--)
                sign[0] += (vect[readedSignes++] * pow(2, j));
            return true;
        } else if (vect[readedSignes] == 1 && vect[readedSignes + 1] == 1 && vect[readedSignes + 2] == 0 &&
                   vect[readedSignes + 8] == 1 && vect[readedSignes + 9] == 0) {
            for (int i = 0; i < 2; i++)
                for (int j = 7; j >= 0; j--)
                    sign[i] += vect[readedSignes++] * pow(2, j);
            return true;
        } else if (vect[readedSignes] == 1 && vect[readedSignes + 1] == 1 && vect[readedSignes + 2] == 1 &&
                   vect[readedSignes + 3] == 0 && vect[readedSignes + 8] == 1 && vect[readedSignes + 9] == 0 &&
                   vect[readedSignes + 16] == 1 && vect[readedSignes + 17] == 0) {
            for (int i = 0; i < 3; i++)
                for (int j = 7; j >= 0; j--)
                    sign[i] += vect[readedSignes++] * pow(2, j);
            return true;
        } else if (vect[readedSignes] == 1 && vect[readedSignes + 1] == 1 && vect[readedSignes + 2] == 1 &&
                   vect[readedSignes + 3] == 1 && vect[readedSignes + 4] == 0 && vect[readedSignes + 8] == 1 &&
                   vect[readedSignes + 9] == 0 && vect[readedSignes + 16] == 1 && vect[readedSignes + 17] == 0 &&
                   vect[readedSignes + 24] == 1 && vect[readedSignes + 25] == 0) {
            for (int i = 0; i < 4; i++)
                for (int j = 7; j >= 0; j--)
                    sign[i] += vect[readedSignes++] * pow(2, j);
            return true;
        } else { return false; }
    }
};


bool decompressingOfChunks ( const vector<int> & vect ,const CTree & tree, int & readedSignes, ofstream & ofs, const int vectorSize ) {
    bool endOfTheCycle = true;

    do {
        if ( vect[readedSignes++] == 1 ) {
            for ( size_t i = 0; i < 4096; i ++) {
                bool end = true;
                CNode * neededElement = tree . start;
                do {
                        if ( readedSignes > vectorSize && neededElement -> right != nullptr && neededElement -> left != nullptr )
                            return false;
                    if ( neededElement -> right != nullptr && vect[readedSignes] == 1 )
                    {
                        readedSignes++;
                        neededElement = neededElement -> right;
                    }
                    else if ( neededElement -> left != nullptr && vect[readedSignes] == 0 )
                    {
                        readedSignes++;
                        neededElement = neededElement -> left;
                    } else {
                        end = false;
                    }
                } while ( end );
                        for ( int i = 0; i < 4; i ++) {
                            if ( neededElement -> data[i] == '\0' )
                                break;
                            else
                                ofs << neededElement -> data[i];
                        }
            }
        } else {
            int numberOfSignes = 0, numberOfSignesTest = 0;

            for (int j = 11; j >= 0; j--)
                numberOfSignes += vect[readedSignes++] * pow(2, j);

            for ( int i = 0; i < numberOfSignes; i ++) {
                bool end = true;
                CNode * neededElement = tree . start;
                do {
                    if ( readedSignes > vectorSize && neededElement -> right != nullptr && neededElement -> left != nullptr )
                        return false;
                    if ( neededElement -> left != nullptr && vect[readedSignes] == 0 )
                    {
                        readedSignes++;
                        neededElement = neededElement -> left;

                    }
                    else if ( neededElement -> right != nullptr && vect[readedSignes] == 1 )
                    {
                        readedSignes++;
                        neededElement = neededElement -> right;
                    } else {
                        numberOfSignesTest++;
                        end = false;
                    }
                } while ( end );
                        for ( int i = 0; i < 4; i ++) {
                            if (neededElement->data[i] == '\0' && i != 0)
                                break;
                            else
                            {
                                if ( ofs.fail() )
                                    return false;
                                ofs << neededElement->data[i];
                            }
                        }
            }
            if ( numberOfSignes != numberOfSignesTest )
                return false;
            endOfTheCycle = false;
        }
    } while ( endOfTheCycle );
    return true;
}


bool decompressFile ( const char * inFileName, const char * outFileName ) {
    ifstream ifs (inFileName, ios::in | ios::binary);
    if ( ifs.fail() || !ifs.is_open() )
    {
        ifs.close();
        return false;
    }

    vector<int> vect;
    int readedSignes = 0;

    for (char x; (ifs.get(x));) {
        if ( ifs.fail() )
        {
            ifs.close ();
            return false;
        }
        for (int i = 7; i >= 0; i--)
        {
            vect.push_back((x >> i) & 1);
        }
    }
    int vectorSize = vect.size () - 1;


    ifs.close();

    CTree decoderTree = CTree ();
    if ( !decoderTree . recursionCreation( decoderTree . start, vect, ++readedSignes ) )
        return false;


    ofstream ofs (outFileName, ios::out | ios::binary);

    if ( ofs.fail() || !ofs.is_open() || !ofs )
    {
        remove ( outFileName );
        ofs.close();
        return false;
    }

    if ( !decompressingOfChunks (vect, decoderTree, readedSignes, ofs, vectorSize) )
    {
        remove ( outFileName );
        ofs.close();
        return false;
    }
    ofs.close();

    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
    ifstream ifs1 ( fileName1);
    ifstream ifs2 ( fileName2);

    if ( !ifs1.is_open() || ifs1.fail() || !ifs2.is_open() || ifs2.fail() )
        return false;

    for ( string x2, x1; getline ( ifs1, x1 ), getline ( ifs2, x2); )
            if ( x1 != x2)
            {
                cout << x1 << endl;
                cout << x2 << endl;
                ifs1.close();
                ifs2.close();
                return false;
            }
    ifs1.close();
    ifs2.close();
    return true;
}

int main ( void )
{
/*

    assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );


    assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );


    assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );


    assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );
    assert ( decompressFile ( "tests/tests/extra2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/tests/extra2.orig", "tempfile" ) );
    assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );
*/
    assert ( decompressFile ( "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/progtest/domaci_ulohy/du1/tests/extra4.huf", "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/progtest/domaci_ulohy/du1/tempfile" ) );
    assert ( identicalFiles ( "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/progtest/domaci_ulohy/du1/tests/extra4.orig", "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/progtest/domaci_ulohy/du1/tests/tempfile" ) );
/*
    assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );
*/


    return 0;
}
#endif /* __PROGTEST__ */
