//
// =========================================================================
//
//       Filename:  fairshare.h
//
//    Description:  Declares functions and global variables specific to
//                  main program.
//
//        Version:  1.0
//        Created:  02/24/2015 12:48:38 PM
//       Revision:  none
//       Compiler:  g++
//
//          Usage:  <+USAGE+>
//
//         Output:  <+OUTPUT+>
//
//         Author:  Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  
//
// =========================================================================
//
#ifndef  FAIRSHARE_INC
#define  FAIRSHARE_INC

//--------------------------------------------------------------------------
//  includes
//--------------------------------------------------------------------------
// {{{
// standard libraries
#include <string>                    // string handling

// external libraries
#include <boost/program_options.hpp> // allows to set program options
//}}}

//--------------------------------------------------------------------------
//  function declarations
//--------------------------------------------------------------------------
void   DisplayHelp (const char *execName,
                    const boost::program_options::options_description opts);

void   GetArgsToMain  ( int ac, char *av[]);
void   ParseIniFile   ( const std::string & fileName);

void   DisplayResults ( );
double CalculateRatio ( const double income_1, const double income_2 );
void   CheckIncomeIsNonZeroOrExit ( );
//--------------------------------------------------------------------------
//  global variables
//--------------------------------------------------------------------------
extern std::string person_1;
extern std::string person_2;

extern double income_1;
extern double income_2;

extern double rent; 
extern double utilities;
extern double telecom;
extern double food;
#endif   //---- #ifndef FAIRSHARE_INC  -----
