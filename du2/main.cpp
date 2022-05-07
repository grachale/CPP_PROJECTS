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




class CVATRegister {
public:
    CVATRegister(void) = default;

    ~CVATRegister(void) {
        for (const m_Company *it: compSortByNamesAddr)
            delete it;
    };

    bool newCompany(const string &name,
                    const string &addr,
                    const string &taxID);

    bool cancelCompany(const string &name,
                       const string &addr);

    bool cancelCompany(const string &taxID);

    bool invoice(const string &taxID,
                 unsigned int amount);

    bool invoice(const string &name,
                 const string &addr,
                 unsigned int amount);

    bool audit(const string &name,
               const string &addr,
               unsigned int &sumIncome) const;

    bool audit(const string &taxID,
               unsigned int &sumIncome) const;

    bool firstCompany(string &name,
                      string &addr) const;

    bool nextCompany(string &name,
                     string &addr) const;

    unsigned int medianInvoice(void) const;

private:

    struct m_Company {
        m_Company(const string &nameLow = "\0",
                  const string &addLow = "\0",
                  const string &taxID = "\0",
                  const string &name = "\0",
                  const string &addr = "\0")
                : s_Name(name),
                  s_Addr(addr),
                  s_TaxID(taxID),
                  s_NameLowCase(nameLow),
                  s_AddrLowCase(addLow),
                  sumIncome(0) {
        }

        string s_Name;
        string s_Addr;
        string s_TaxID;

        string s_NameLowCase;
        string s_AddrLowCase;
        unsigned int sumIncome;                     // sum of all incomes for certain company.
    };

    vector<m_Company *>  compSortByNamesAddr;       // vector with pointers to m_Company, which is sorted by names and addresses to fastly search needed company with the company's name and address.
    vector<m_Company *>  compSortByIds;             // vector with pointers to m_Company, which is sorted by ids to fastly search needed company with the company's id.
    vector<unsigned int> incomes;                   // vector with all incomes.


/**
 * Compares two companies (with their low-case names, if they are the same,
it compares with their low-case addresses, because companies can't have simultaneously
the same addresses and names.
 *
 * @param first pointer to the first company
 * @param second pointer to the second company
 *
 * @return true (the first company is "smaller) or false (the first company is "bigger")
*/
    static bool compareForNamesAddr ( const m_Company *first, const m_Company *second ) {
        return ( strcmp ( first -> s_NameLowCase . c_str (), second -> s_NameLowCase . c_str () ) < 0 ) ||
               ( strcmp ( first -> s_NameLowCase . c_str (), second -> s_NameLowCase . c_str () ) == 0  &&
                 strcmp ( first -> s_AddrLowCase . c_str (), second -> s_AddrLowCase . c_str () ) < 0 );
    }
/**
 * Compare two companies with their ids.
 *
 * @param first pointer to the first company
 * @param second pointer to the second company
 *
 * @return true (the first company is "smaller) or false (the first company is "bigger")
*/
    static bool compareForIds(const m_Company *first, const m_Company *second) {
        return strcmp(first->s_TaxID.c_str(), second->s_TaxID.c_str()) < 0;
    }

};

/**
* Low case the input string.
*
* @param word the word
* @return word in low case
*/
string toLowCase ( const string &word ) {
    string wordInLowCase;
    for (const char &it: word)
        wordInLowCase . push_back( tolower( it ) );

    return wordInLowCase;
}
/**
 * Adding a new company.
 *
 * @param name the name of the company
 * @param addr the address of the company
 * @param taxID the taxID of the company
 *
 * @return true (successfully added) or false (company wasn't added because it has been added already)
*/
bool CVATRegister::newCompany ( const string &name, const string &addr, const string &taxID) {

    string newNameLow = toLowCase( name );
    string newAddrLow = toLowCase( addr );

    // checking whether the company was added somewhere or not
    for (const m_Company *company: compSortByNamesAddr)
    {
        if ( ( company -> s_NameLowCase == newNameLow  && company -> s_AddrLowCase == newAddrLow ) ||
                 taxID == company -> s_TaxID )
            return false;
    }

    m_Company *newCompony = new m_Company( newNameLow, newAddrLow, taxID, name, addr );

    auto it = lower_bound(compSortByIds.begin(), compSortByIds.end(), newCompony, compareForIds);
    auto it2 = lower_bound(compSortByNamesAddr.begin(), compSortByNamesAddr.end(), newCompony, compareForNamesAddr);

    compSortByNamesAddr . insert ( it2, newCompony);
    compSortByIds . insert ( it, newCompony);

    return true;
}
/**
 * Cancel the company with its name and addr.
 *
 * @param name the name of the company
 * @param addr the address of the company
 *
 * @return true (successfully canceled) or false (company wasn't canceled because it hasn't been found)
*/
bool CVATRegister::cancelCompany( const string & name, const string & addr )
{
    string cancelNameLow = toLowCase ( name );
    string cancelAddrLow = toLowCase( addr );

    m_Company * neededCompony = new m_Company ( cancelNameLow, cancelAddrLow );

    auto it = lower_bound ( compSortByNamesAddr . begin (), compSortByNamesAddr . end (), neededCompony, compareForNamesAddr );

    // checking if we find something and check if the found company is  what we were looking for (has the same name and address)
    if ( it == compSortByNamesAddr . end () || (*it) -> s_NameLowCase != cancelNameLow || (*it) -> s_AddrLowCase != cancelAddrLow )
    {
        delete neededCompony;
        return false;
    }

    // searching for needed company in the vector sorted by taxID
    neededCompony -> s_TaxID = (*it) -> s_TaxID;
    auto it2 = lower_bound ( compSortByIds . begin (), compSortByIds . end (), neededCompony, compareForIds );

    delete (*it);                                         // free space of canceled company
    delete neededCompony;                                 // free space of temporary company for searching
    compSortByNamesAddr . erase ( it );
    compSortByIds . erase ( it2 );

    return true;
}
/**
 * Cancel the company with its taxID.
 *
 * @param taxID the taxID of the company
 *
 * @return true (successfully canceled) or false (company wasn't canceled because it hasn't been found)
*/
bool CVATRegister::cancelCompany ( const string & taxID )
{
    m_Company * neededCompony = new m_Company ();
    neededCompony -> s_TaxID = taxID;

    auto it2 = lower_bound ( compSortByIds . begin (), compSortByIds . end (), neededCompony, compareForIds );
    // checking if we find something and check if the found company is what we were looking for (has the same id)
    if ( it2 == compSortByIds . end () || taxID != (*it2) -> s_TaxID)
    {
        delete neededCompony;
        return false;
    }

    // searching for needed company in the vector sorted by its name and address
    neededCompony -> s_AddrLowCase = (*it2) -> s_AddrLowCase;
    neededCompony -> s_NameLowCase = (*it2) -> s_NameLowCase;
    auto it = lower_bound ( compSortByNamesAddr . begin (), compSortByNamesAddr . end (), neededCompony, compareForNamesAddr );

    delete (*it);
    delete neededCompony;                                 // free space of temporary company for searching
    compSortByNamesAddr . erase ( it );
    compSortByIds . erase ( it2 );
    return true;
}
/**
 * Invoice the amount of company with certain taxID.
 *
 * @param taxID the taxID of the company
 * @param amount the amount of income
 *
 * @return true (successfully added invoce) or false (company hasn't been found)
*/
bool CVATRegister::invoice ( const string & taxID, unsigned int amount )
{
    m_Company * neededCompony = new m_Company ();

    neededCompony -> s_TaxID = taxID;
    auto it2 = lower_bound ( compSortByIds . begin (), compSortByIds . end (), neededCompony, compareForIds );
    // checking if we find something and check if the found company is what we were looking for (has the same id)
    if ( it2 == compSortByIds . end () || (*it2) ->s_TaxID != taxID )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;                                 // free space of temporary company for searching

    (*it2) -> sumIncome += amount;                        // adding income to the company
    auto it = lower_bound( incomes . begin (), incomes . end(), amount);
    // in case income must be at the end of our vector, which stores incomes, we need to push the income, otherwise we need to insert it with an iterator.
    if ( it == incomes . end() )
    {
        incomes.push_back( amount );
        return true;
    }
    incomes .insert( it, amount);
    return true;
}
/**
 * Invoice the amount of company with certain name and address.
 *
 * @param name the name of the company
 * @param addr the address of the company
 * @param amount the amount of income
 *
 * @return true (successfully added invoce) or false (company hasn't been found)
*/
bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
    string invoiceNameLow = toLowCase (  name );
    string invoiceAddrLow = toLowCase( addr );
    m_Company * neededCompony = new m_Company ( invoiceNameLow, invoiceAddrLow );

    auto it2 = lower_bound ( compSortByNamesAddr . begin (), compSortByNamesAddr . end (), neededCompony, compareForNamesAddr );
    // checking if we find something and check if the found company is  what we were looking for (has the same name and address)
    if ( it2 == compSortByNamesAddr . end () || (*it2) -> s_NameLowCase != invoiceNameLow || (*it2) -> s_AddrLowCase != invoiceAddrLow )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;                                 // free space of temporary company for searching

    (*it2) -> sumIncome += amount;                        // adding income to the company

    auto it = lower_bound( incomes . begin (), incomes . end(), amount);
    // in case income must be at the end of our vector, which stores incomes, we need to push the income, otherwise we need to insert it with an iterator.
    if ( it == incomes . end() )
    {
        incomes.push_back( amount );
        return true;
    }
    incomes .insert( it, amount);
    return true;
}
/**
 * Find the sum of all incomes of company with certain name and address and writes it to the sumIncome parameter.
 *
 * @param name the name of the company
 * @param addr the address of the company
 * @param sumIncome the sum of incomes of the company
 *
 * @return true (successfully found the audit) or false (company hasn't been found)
*/
bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
    string auditNameLow = toLowCase ( name );
    string auditAddrLow = toLowCase( addr );
    m_Company * neededCompony = new m_Company ( auditNameLow, auditAddrLow );

    auto it = lower_bound ( compSortByNamesAddr . begin (), compSortByNamesAddr . end (), neededCompony, compareForNamesAddr );
    // checking if we find something and check if the found company is  what we were looking for (has the same name and address)
    if ( it == compSortByNamesAddr . end () || (*it) -> s_NameLowCase != auditNameLow || (*it) -> s_AddrLowCase != auditAddrLow )
    {
        delete neededCompony;
        return false;
    }

    delete neededCompony;                                 // free space of temporary company for searching

    sumIncome = (*it) -> sumIncome;

    return true;
}
/**
 * Finds the sum of all incomes of company with certain taxID and writes it to the sumIncome parameter.
 *
 * @param taxID the taxID of the company
 * @param sumIncome the sum of incomes of the company
 *
 * @return true (successfully found the audit) or false (company hasn't been found)
*/
bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
    m_Company * neededCompony = new m_Company ();
    neededCompony -> s_TaxID = taxID;

    auto it2 = lower_bound ( compSortByIds . begin (), compSortByIds . end (), neededCompony, compareForIds );
    // checking if we find something and check if the found company is what we were looking for (has the same id)
    if ( it2 == compSortByIds . end () || (*it2) ->s_TaxID != taxID)
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;                                 // free space of temporary company for searching

    sumIncome = (*it2) -> sumIncome;
    return true;
}
/**
 * Finds the company, which is on the top of alphabetical order of names (in case names are the same - the address decides)
 *
 * @param name the name of the company
 * @param addr the address of the company
 *
 * @return true (successfully found "the first" company) or false (company hasn't been found)
*/
bool CVATRegister::firstCompany( string & name, string & addr ) const
{
    if ( compSortByNamesAddr . empty() )
        return false;

    name = compSortByNamesAddr[0] -> s_Name;
    addr = compSortByNamesAddr[0] -> s_Addr;

    return true;
}
/**
 * Finds the company, which is after the certain company in  alphabetical order of names (in case names are the same - the address decides)
 *
 * @param name the name of the company
 * @param addr the address of the company
 *
 * @return true (successfully found "the next" company) or false (company hasn't been found)
*/
bool CVATRegister::nextCompany( string & name, string & addr ) const
{
    string nextNameLow = toLowCase (  name );
    string nextAddrLow = toLowCase( addr );

    m_Company * neededCompony = new m_Company ( nextNameLow, nextAddrLow );

    auto it2 = lower_bound ( compSortByNamesAddr . begin (), compSortByNamesAddr . end (), neededCompony, compareForNamesAddr );
    // checking if we find something, the next company exists and check if the found company is what we were looking for (has the same name and address)
    if ( it2 == compSortByNamesAddr . end () || (*it2) -> s_NameLowCase != nextNameLow || (*it2) -> s_AddrLowCase != nextAddrLow || (++it2) == compSortByNamesAddr . end () )
    {
        delete neededCompony;
        return false;
    }
    delete neededCompony;                                 // free space of temporary company for searching

    name = (*it2) -> s_Name;
    addr = (*it2) -> s_Addr;

    return true;
}
/**
 * Finds the median of all invoices of all companies which are in decreasing order.
 *
 * @return amount of median invoice
*/
unsigned int  CVATRegister::medianInvoice  ( void ) const
{
    if ( incomes . empty() )
        return 0;

    return incomes [ (int) incomes . size() / 2 ];
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
