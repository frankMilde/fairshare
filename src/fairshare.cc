//
// =========================================================================
//
//       Filename:  rent-share.cc
//
//    Description:  Calculates the expenses of each room mate such that
//                  everybody pays the same percentage of ones income.
//
//        Version:  1.0
//        Created:  02/23/2015 08:14:57 PM
//       Revision:  none
//       Compiler:  g++
//
//          Usage:  ./fairshare
//
//         Output:  
//                  | Income        | Rent          | Utilities     | Telephone     | Food          | Total         |
//   ================================================================================================================
//            Costs |               |       1000.00 |         50.00 |         20.00 |        200.00 |       1270.00 |
//   ---------------+---------------+---------------+---------------+---------------+---------------+---------------|
//              Max |       1000.00 |        333.33 |         16.67 |          6.67 |         66.67 |        423.33 |
//   ---------------+---------------+---------------+---------------+---------------+---------------+---------------|
//             Maxi |       2000.00 |        666.67 |         33.33 |         13.33 |        133.33 |        846.67 |
//  
//  
//  Every person pays a fair share of 42.33% of her/his income.
//  
//
//         Author:  Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  
//
//           TODO:  - create person and expenses struct
//                  - create settings file with user input
//                  - user should be able to provide arbitrary number of
//                    self named expenses
//                  - extent to use with arbitrary number of individuals
// =========================================================================

//--------------------------------------------------------------------------
//  includes
//--------------------------------------------------------------------------
// {{{
// standard libraries
#include <iomanip>    // i/o stream manipulatonen, e.g. setprecision
#include <iostream>   // input/output streams, e.g. std::cout and cin
#include <fstream>    // filestreams to read and write data to files
#include <string>     // string handling
#include <stdexcept>  // for exception handling, e.g. std::out_of_range
#include <sys/stat.h> // shell instructions 

// external libraries
#include <boost/program_options.hpp>     // allows to set program options
#include <boost/property_tree/ptree.hpp> // parse ini files
#include <boost/property_tree/ini_parser.hpp>

// files
#include "fairshare.h"
#include "helper-functions.h"
#include "global-constants.h"
//}}}

//--------------------------------------------------------------------------
//  global variables definitions
//--------------------------------------------------------------------------
double rent      = 0.0;
double utilities = 0.0;
double telecom   = 0.0;
double food      = 0.0;
double income_1  = 0.0;
double income_2  = 0.0;

std::string person_1;
std::string person_2;

// =========================================================================
//   Main
// =========================================================================
int main(int argc, char *argv[]) {

  ParseIniFile(kIniFileName);
  GetArgsToMain(argc, argv);

  CheckIncomeIsNonZeroOrExit();
  
  DisplayResults();

  return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------
//  function definitions
//--------------------------------------------------------------------------
void GetArgsToMain(int ac, char *av[]) {
  namespace po = boost::program_options; // just for convenience
  try {
    // define and parse program options
    po::options_description opts("\033[1mOPTIONS\033[0m");
    opts.add_options()
      ("help,h", "produce help message")
      ("income1,1",
       po::value<double>(&income_1 ),
       "sets first income") 
      ("income2,2",
       po::value<double>(&income_2 ),
       "sets second income") 
      ("rent,r",
       po::value<double>(&rent ),
       "sets rent") 
      ("util,u",
       po::value<double>(&utilities ),
       "sets utilities") 
      ("food,f",
       po::value<double>(&food ),
       "sets food expenses") 
      ("tele,t",
       po::value<double>(&telecom ),
       "sets telecommunication expenses") 
      ;

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
        options(opts).run(), vm);

    if (vm.count("help")) {
      DisplayHelp(av[0],opts);
      exit(EXIT_SUCCESS);
    }       //----  end if -----

    po::notify(vm);
  }       //----  end try -----

  catch(std::exception& e) {
    std::cerr <<  "ERROR: " << e.what() << "\n\n";
    std::cerr << "Exiting program.\n\n";
    exit(EXIT_FAILURE);
  }       //----  end catch  -----

  catch(...) {
    std::cerr << "ERROR: " << "Exception of unknown type!\n\n";
    std::cerr << "Exiting program.\n\n";
    exit(EXIT_FAILURE);
  }       //----  end catch -----

}   // -----  end of function getArgsToMain  -----

void DisplayHelp (const char *execName, 
    const boost::program_options::options_description opts) {
  std::cout << bold << "NAME:"        << normal           << std::endl;
  std::cout << "  " << execName << " - "
    << "gives the share of expenses depending on income"
    << std::endl
    << std::endl;

  std::cout << bold << "SYNOPSIS:"    << normal           << std::endl;
  std::cout << "  " << execName << " [options]" //arg [...]" 
    << std::endl << std::endl;

  std::cout << bold << "DESCRIPTION:" << normal           << std::endl;
  std::cout 
    << "  Uses simple percentile arithmetics to calculate the share each " 
    << std::endl
    << "  person has to pay for monthly expenses in a way that everybody " 
    << std::endl
    << "  pays the same percentage of ones income." 
    << std::endl
    << std::endl;

  std::cout << opts;
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << bold << "EXAMPLES:"    << normal           << std::endl;
  std::cout << "  " << execName << std::endl; 
  std::cout << std::endl
       << "    Uses parameters given by settings.ini" 
       << std::endl
       << std::endl
       << std::endl;
  std::cout << "  " << execName << " -r 1500 -2000 " << std::endl; 
  std::cout << std::endl
       << "    Ignores the income 2 and the rent from settings.ini and uses"
       << std::endl
       << "    the ones provided by argument the command line arguments "
       << std::endl
       << "    instead."
       << std::endl
       << std::endl
       << std::endl;

}   // -----  end of function DisplayHelp  -----

void ParseIniFile (const std::string & fileName) {

  CheckFileExistsOrExit (fileName);
  
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(fileName, pt);

  try {
    rent      = pt.get<double> ("expenses.rent");
    utilities = pt.get<double> ("expenses.utilities");
    telecom   = pt.get<double> ("expenses.telecom");
    food      = pt.get<double> ("expenses.food");

    income_1  = pt.get<double> ("person1.income");
    income_2  = pt.get<double> ("person2.income");

    person_1  = pt.get<std::string> ("person1.name");
    person_2  = pt.get<std::string> ("person2.name");
  }
  catch (std::exception const& e) {
    DisplayError(e.what());
    exit(EXIT_FAILURE);
  }

}   // -----  end of function ParseIniFileWavefunction  -----

double CalculateRatio (const double income_1, const double income_2 ) {
  const double alpha = income_1/income_2;
  return 1./(income_2*(1.+alpha));
}  // -----  end of function CalculateIncomeRatio  -----

void CheckIncomeIsNonZeroOrExit ( ) {

  bool income_1_isZero = false;
  bool income_2_isZero = false;

  if (income_1 <= 0. && income_1 >= 0.) {
    income_1_isZero = true;
  }
  if (income_2 <= 0. && income_2 >= 0.) {
    income_2_isZero = true;
  }

  if(income_1_isZero || income_2_isZero) {
    DisplayError("Both incomes have to be non zero.");
    exit(EXIT_FAILURE);
  }
}  // -----  end of function CheckIncomeIsNonZeroOrExit  -----

void DisplayResults (  ) {

  double ratio = CalculateRatio(income_1,income_2);
  double percentTotal = (rent+utilities+telecom+food)*ratio;

  using namespace std;

  int width = 15;
  ostringstream topLine;
  ostringstream separatorLine;

  // We have to add +1 to the width, for the top line, bottom line,
  // separator line, since the output of separator symbol "+" is added to
  // the total width of the line, but for us w means the width of the "inner"
  // column
  topLine
    << " "
    << setfill('=')
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setw(width+1)  << "="
    << setfill(' ')
    << endl;

  separatorLine 
    << " "
    << setfill('-')
    << setw(width+1)  << "+"
    << setw(width+1)  << "+"
    << setw(width+1)  << "+"
    << setw(width+1)  << "+"
    << setw(width+1)  << "+"
    << setw(width+1)  << "+"
    << setw(width+1)  << "|"
    << setfill(' ')
    << endl;



  // here we have to substract -1 from the width, as we have as a separator
  // not simply a one char "|", but two chars " |"
  cout 
    << std::endl
    << std::endl
    << " "
    << left
    << setw(width-1)
    << " " << " |" 
    << setw(width-1) << setprecision(2) << fixed << left
    << " Income" << " |" 
    << setw(width-1) << fixed << left
    << " Rent" << " |" 
    << setw(width-1) << fixed << left
    << " Utilities" << " |" 
    << setw(width-1) << fixed << left
    << " Telecom" << " |" 
    << setw(width-1) << fixed << left
    << " Food" << " |" 
    << setw(width-1) << fixed << left
    << " Total" << " |" 
    << std::endl
    << topLine.str();
  cout 
    << " "
    << right
    << setw(width-1)
    << "Costs" << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << " " <<  " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << rent << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << utilities << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << telecom << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << food << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << (rent + utilities + telecom + food) << " |" 
    << std::endl
    << separatorLine.str();
  cout 
    << " "
    << right
    << setw(width-1)
    << person_1 << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 * ratio * rent << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 * ratio * utilities << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 * ratio * telecom << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 * ratio * food << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_1 * ratio * (rent + utilities + telecom + food) << " |" 
    << std::endl
    << separatorLine.str();
  cout 
    << " "
    << right
    << setw(width-1)
    << person_2 << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 * ratio * rent << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 * ratio * utilities << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 * ratio * telecom << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 * ratio * food << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << income_2 * ratio * (rent + utilities + telecom + food) << " |" 
    << std::endl;

  std::cout
    << std::endl
    << std::endl
    <<"Every person pays a fair share of "
    << bold << 100*percentTotal <<"%" << normal
    <<" of her/his income."
    << std::endl;
}  // -----  end of function DisplayResults  -----
