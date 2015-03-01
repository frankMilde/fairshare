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
//  pods
//--------------------------------------------------------------------------
struct person {
  std::string name;
  double      income;
};  // -----  end of struct person  -----
struct expense {
  std::string name;
  double      cost;
};  // -----  end of struct expense  -----

typedef struct person Person;
typedef struct expense Expense;

//--------------------------------------------------------------------------
//  function declarations
//--------------------------------------------------------------------------
void   DisplayHelp (const char *execName,
                    const boost::program_options::options_description opts);

void   CheckIncomeIsNonZeroOrExit (const std::vector<Person> & p);
void   GetArgsToMain  (int ac, char *av[]);
void   ParseIniFile   (const std::string & fileName);

int    LongestString  (const std::vector<Expense> & e);
int    LongestString  (const std::vector<Person > & p);

double CalculateRatio (const std::vector<Person > & p);

void   DisplayResults (const std::vector<Person> & p, const std::vector<Expense> &e);
void   DisplayInputs  (const std::vector<Person> & p, const std::vector<Expense> &e);
#endif   //---- #ifndef FAIRSHARE_INC  -----
