#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
public:
    CVATRegister   ( void ) = default;
    ~CVATRegister  ( void )
    {
        for ( unsigned long int i = 0; i < names . size(); ++i)
            delete names[i];
    };
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    bool          cancelCompany  ( const string    & taxID );
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    unsigned int  medianInvoice  ( void ) const;

private:

    struct m_Company
    {
        m_Company ( const string & name = "\0", const string & addr = "\0", const string & taxID = "\0" )
                : s_Name ( name ),
                  s_Addr ( addr ),
                  s_TaxID ( taxID ),
                  sumIncome ( 0 )
        {}
        string s_Name;
        string s_Addr;
        string s_TaxID;
        unsigned int sumIncome;
    };

    vector <m_Company *> names;
    vector <m_Company *> ids;
    vector <unsigned  int> incomes;

    static bool compare1 ( const m_Company * first, const m_Company * second ) {
        string name1, name2;
        for ( unsigned long int i = 0; i < first -> s_Name . length(); i++ )
        {
            name1.push_back(tolower(first -> s_Name[i]));
        }
        for ( unsigned long int i = 0; i < second -> s_Name .length(); i++ )
        {
            name2.push_back(tolower(second -> s_Name[i]));
        }
        string add1, add2;
        for ( unsigned long int i = 0; i < first -> s_Addr . length(); i++ )
        {
            add1.push_back(tolower(first -> s_Addr[i]));
        }
        for ( unsigned long int i = 0; i < second -> s_Addr . length(); i++ )
        {
            add2.push_back(tolower(second -> s_Addr[i]));
        }
        return ( strcmp( name1 . c_str(), name2 . c_str() ) < 0 ) || ( strcmp( name1 . c_str(), name2 . c_str() ) == 0 && strcmp( add1 . c_str(), add2 . c_str() ) < 0 );
    }

    static bool compare2 ( const m_Company * first, const m_Company * second )
    {
        return strcmp ( first -> s_TaxID . c_str(), second -> s_TaxID . c_str() ) < 0;
    }

    bool areTheSame ( const string & name1, const string & name2, const string & add1, const string & add2)const {
        string name1S, add1S, name2S, add2S;
        for (unsigned long int i = 0; i < name1.size(); i++) {
            name1S.push_back(tolower(name1[i]));
        }
        for (unsigned long int i = 0; i < add1.size(); i++) {
            add1S.push_back(tolower(add1[i]));
        }
        for (unsigned long int i = 0; i < name2.size(); i++) {
            name2S.push_back(tolower(name2[i]));
        }
        for (unsigned long int i = 0; i < add2.size(); i++) {
            add2S.push_back(tolower(add2[i]));
        }
        if (strcmp(name1S.c_str(), name2S.c_str()) == 0 && strcmp(add1S.c_str(), add2S.c_str()) == 0)
            return true;
        else
            return false;
    }

};

string toLowCase ( const string & word )
{
    string wordInLowCase;
    for ( unsigned long int i = 0; i < word . size() ; i++ )
    {
        wordInLowCase.push_back(tolower(word[i]));
    }
    return wordInLowCase;
}


bool CVATRegister::newCompany(const string &name, const string &addr, const string &taxID) {

    string name1 = toLowCase( name );
    string addr1 = toLowCase( addr );

    for ( unsigned long int j = 0; j < names . size (); j++ )
    {
        string name2, add2;
        for ( unsigned long int i = 0; i < names[j] -> s_Name . length(); i++ )
        {
            name2.push_back(tolower(names[j] -> s_Name[i]));
        }
        for ( unsigned long int i = 0; i < names[j] -> s_Addr . length(); i++ )
        {
            add2.push_back(tolower(names[j] -> s_Addr[i]));
        }
        if ( ( strcmp (name1 .c_str(), name2 .c_str() ) == 0 && strcmp( addr1 . c_str(), add2 . c_str() ) == 0 ) || strcmp( taxID . c_str(), names[j] ->s_TaxID . c_str() ) == 0 ) {
            return false;
        }

    }
    m_Company *newCompony = new m_Company(name, addr, taxID);

    auto it = lower_bound(ids.begin(), ids.end(), newCompony, compare2);
    auto it2 = lower_bound(names.begin(), names.end(), newCompony, compare1);

    names .insert( it2, newCompony);
    ids .insert( it, newCompony);


    return true;
}

bool CVATRegister::cancelCompany( const string & name, const string & addr )
{
    m_Company * neededCompony = new m_Company ( name, addr );
    auto it = lower_bound ( names . begin (), names . end (), neededCompony, compare1 );
    if ( it == names . end () || !areTheSame((*it) ->s_Name, name, (*it) -> s_Addr, addr))
    {
        delete neededCompony;
        return false;
    }
    neededCompony -> s_TaxID = (*it) -> s_TaxID;
    auto it2 = lower_bound ( ids . begin (), ids . end (), neededCompony, compare2 );
    if (it2 == ids . end() )
    {
        delete neededCompony;
        return false;
    }
    delete (*it);
    delete neededCompony;
    names . erase ( it );
    ids . erase ( it2 );
    return true;
}

bool CVATRegister::cancelCompany ( const string & taxID )
{
    m_Company * neededCompony = new m_Company ();
    neededCompony -> s_TaxID = taxID;
    auto it2 = lower_bound ( ids . begin (), ids . end (), neededCompony, compare2 );
    if ( it2 == ids . end () || taxID != (*it2) ->s_TaxID)
    {
        delete neededCompony;
        return false;
    }
    neededCompony -> s_Addr = (*it2) -> s_Addr;
    neededCompony -> s_Name = (*it2) -> s_Name;
    auto it = lower_bound ( names . begin (), names . end (), neededCompony, compare1 );
    delete (*it);
    delete neededCompony;
    names . erase ( it );
    ids . erase ( it2 );
    return true;
}
bool CVATRegister::invoice ( const string & taxID, unsigned int amount )
{
    m_Company * neededCompony = new m_Company ();
    neededCompony -> s_TaxID = taxID;
    auto it2 = lower_bound ( ids . begin (), ids . end (), neededCompony, compare2 );
    if ( it2 == ids . end () || (*it2) ->s_TaxID != taxID )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;
    (*it2) -> sumIncome += amount;
    auto it = lower_bound( incomes . begin (), incomes . end(), amount);
    if ( it == incomes . end() )
    {
        incomes.push_back( amount );
        return true;
    }
    incomes .insert( it, amount);
    return true;
}
bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
    m_Company * neededCompony = new m_Company ( name, addr );
    auto it2 = lower_bound ( names . begin (), names . end (), neededCompony, compare1 );
    if ( it2 == names . end () || !areTheSame((*it2) ->s_Name, name, (*it2) -> s_Addr, addr) )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;
    (*it2) -> sumIncome += amount;
    auto it = lower_bound( incomes . begin (), incomes . end(), amount);
    if ( it == incomes . end() )
    {
        incomes.push_back( amount );
        return true;
    }
    incomes .insert( it, amount);
    return true;
}
bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
    m_Company * neededCompony = new m_Company ( name, addr );
    auto it = lower_bound ( names . begin (), names . end (), neededCompony, compare1 );
    if ( it == names . end () || ! areTheSame((*it) -> s_Name, name, (*it) -> s_Addr, addr))
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;
    sumIncome = (*it) -> sumIncome;
    return true;
}
bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
    m_Company * neededCompony = new m_Company ();
    neededCompony -> s_TaxID = taxID;
    auto it2 = lower_bound ( ids . begin (), ids . end (), neededCompony, compare2 );
    if ( it2 == ids . end () || (*it2) ->s_TaxID != taxID)
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;
    sumIncome = (*it2) -> sumIncome;
    return true;
}
bool CVATRegister::firstCompany( string & name, string & addr ) const
{
    if ( names . empty() )
        return false;
    name = names[0] -> s_Name;
    addr = names[0] -> s_Addr;
    return true;
}
bool CVATRegister::nextCompany( string & name, string & addr ) const
{
    m_Company * neededCompony = new m_Company ( name, addr );
    auto it2 = lower_bound ( names . begin (), names . end (), neededCompony, compare1 );
    if ( it2 == names . end () || !areTheSame((*it2) ->s_Name, name, (*it2) -> s_Addr, addr) || (++it2) == names . end () )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;
    name = (*it2) -> s_Name;
    addr = (*it2) -> s_Addr;
    return true;
}
unsigned int  CVATRegister::medianInvoice  ( void ) const
{
    if ( incomes . empty() )
        return 0;
    return incomes [ (int)incomes . size() / 2 ];
}


#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . invoice ( "666/666", 2000 ) );

    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . audit ( "1234567", sumIncome ) );
    assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
