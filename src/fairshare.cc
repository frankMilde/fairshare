//
// =========================================================================
//
//       Filename:  fairshare.cc
//
//    Description:  Calculates the expenses of each room mate such that
//                  everybody pays the same percentage of ones income.
//
//        Version:  1.1
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
//        License:  GNU General Public License
//      Copyright:  Copyright (c) 2015, Frank Milde
//
//           TODO:  
//                  - create settings file from user input
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
std::vector<Person> persons;
std::vector<Expense> expenses;

// =========================================================================
//   Main
// =========================================================================
int main(int argc, char *argv[]) {

//  if (IniFileExists(kIniFileName)) {
//    ParseIniFile(kIniFileName);
//  } else {
//    CreateIniFile(kIniFileName);
//  }  // -----  end if-else  ----- 

  ParseIniFile(kIniFileName);
  GetArgsToMain(argc, argv);

  CheckIncomeIsNonZeroOrExit(persons);
  
  DisplayResults(persons,expenses);

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
       po::value<double>(&persons[0].income ),
       "sets first income") 
      ("income2,2",
       po::value<double>(&persons[1].income ),
       "sets second income") 
//      ("rent,r",
//       po::value<double>(&rent ),
//       "sets rent") 
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

  catch(std::exception& exc) {
    DisplayError(exc.what());
    exit(EXIT_FAILURE);
  }       //----  end catch  -----

  catch(...) {
    DisplayError("Exception of unknown type!\n\n");
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

  using boost::property_tree::ptree;
  
  ptree pt;
  read_ini(fileName, pt);

  Person p1;
  Person p2;

  try {
    p1.income = pt.get<double> ("person1.income");
    p2.income = pt.get<double> ("person2.income");

    p1.name   = pt.get<std::string> ("person1.name");
    p2.name   = pt.get<std::string> ("person2.name");
  }
  catch (std::exception const& exc) {
    DisplayError(exc.what());
    exit(EXIT_FAILURE);
  }
  persons.push_back(p1);
  persons.push_back(p2);

  // https://stackoverflow.com/questions/6656380/boost-1-46-1-property-tree-how-to-iterate-through-ptree-receiving-sub-ptrees
  // https://stackoverflow.com/questions/16135285/iterate-over-ini-file-on-c-probably-using-boostproperty-treeptree
  ptree expenses_tree = pt.get_child("expenses");

  try {
    for (auto &v : expenses_tree) {
      Expense e;
      e.name = v.first;
      e.cost = StringToDouble(v.second.data());
      expenses.push_back(e);
    }
  }
  catch (std::exception const& exc) {
    DisplayError(exc.what());
    exit(EXIT_FAILURE);
  }

}   // -----  end of function ParseIniFileWavefunction  -----

double CalculateRatio (const std::vector<Person> & persons ) {
  const double alpha = persons[0].income/persons[1].income;
  return 1./(persons[1].income*(1.+alpha));
}  // -----  end of function CalculateIncomeRatio  -----

void CheckIncomeIsNonZeroOrExit (const std::vector<Person> & persons) {

  for (auto p = persons.begin(); p != persons.end(); ++p) {
    if ((*p).income <= 0. && (*p).income >= 0. ) {
      DisplayError("Incomes have to be non zero.");
      exit(EXIT_FAILURE);
    }  // -----  end if  ----- 
  }  // -----  end for  ----- 

}  // -----  end of function CheckIncomeIsNonZeroOrExit  -----

void DisplayInputs (const std::vector<Person> & persons, const std::vector<Expense> & expenses) {
  
  for (auto p = persons.begin(); p != persons.end(); ++p) {
    std::cout << (*p).name << " " << (*p).income << std::endl;
  }  // -----  end for  ----- 
  for (auto e = expenses.begin(); e != expenses.end(); ++e) {
    std::cout <<(*e).name << " " << (*e).cost << std::endl;
  }  // -----  end for  ----- 

}  // -----  end of function DisplayInputs  -----


// ===  FUNCTION  ==========================================================
//         Name:  LongestString
//  Description:  
// =========================================================================
int LongestString (const std::vector<Expense> & expenses) {

  size_t size = 0;

  // go through the names of the expenses
  for (auto e = expenses.begin(); e != expenses.end(); ++e) {
    if ((*e).name.size() > size) {
      size = (*e).name.size();
    }  // -----  end if  ----- 
  }  // -----  end for  ----- 

  // go through the costs of the expenses
  double sumCosts =0.;
  for (auto e = expenses.begin(); e != expenses.end(); ++e) {
      sumCosts += (*e).cost;
  }  // -----  end for  ----- 
    std::ostringstream sstream;
    sstream << std::setprecision(2) << std::fixed << sumCosts;
    std::string costAsString = sstream.str(); 

    if (costAsString.size() > size) {
      size = costAsString.size();
    }  // -----  end if  ----- 

  return static_cast<int>(size);
}  // -----  end of function LongestString  -----

int LongestString (const std::vector<Person> & persons) {

  size_t size = 0;

  // go through the names of the persons
  for (auto p = persons.begin(); p != persons.end(); ++p) {
    if ((*p).name.size() > size) {
      size = (*p).name.size();
    }  // -----  end if  ----- 

    // go through the income of the persons
    std::ostringstream sstream;
    sstream << std::setprecision(2) << std::fixed << (*p).income;
    std::string incomeAsString = sstream.str(); 

    if (incomeAsString.size() > size) {
      size = incomeAsString.size();
    }  // -----  end if  ----- 
  }  // -----  end for  ----- 

  return static_cast<int>(size);
}  // -----  end of function LongestString  -----



// ===  FUNCTION  ==========================================================
//         Name:  DisplayResults
//  Description:  We want to pretty display the following way:
//
//                          | Income  | expense 1   | ... |  ... | last expense | Total   |
//                 ============================================================================
//               Name/Costs |         |     1000.00 | ... |  ... |       500.00 |    0.00 |
//                 ---------+---------+-------------+-----+------+--------------+---------|
//                   Name 1 |    0.00 |        0.00 | ... |  ... |         0.00 |    0.00 |
//                 ---------+---------+-------------+-----+------+--------------+---------|
//                   Name 2 |    0.00 |        0.00 | ... |  ... |         0.00 |    0.00 |
//                
//                First we need to find the longest string among all column
//                headers and entries. This includes "Income", "Total", the
//                persons names and the expenses names and all costs. We
//                pick the longest among these and will add 4 to the column
//                width as we start with a space and end each column with
//                three spaces
//
// =========================================================================
void DisplayResults (const std::vector<Person> & p, const std::vector<Expense> &e) {

  double ratio = CalculateRatio(p);

  using namespace std;

  int width_expense = LongestString(e);
  int width_person  = LongestString(p);
  int width_income  = 10; // widths of the string "Name/Costs"

  int width = width_income;
  if (width_expense > width) {
    width = width_expense;
  }  // -----  end if  ----- 
  if (width_person > width) {
    width = width_person;
  }  // -----  end if  ----- 
  width += 4; // to add a space before the word and three spaces behind

  ostringstream topLine;
  ostringstream separatorLine;

  // We have to add +1 to the width, for the top line, bottom line,
  // separator line, since the output of separator symbol "+" is added to
  // the total width of the line, but for us "width" means the width of the
  // "inner" column
  topLine
    << " "
    << setfill('=')
    << setw(width+1)  << "="  // cost   column
    << setw(width+1)  << "="; // income column
  for (auto i = e.begin(); i != e.end(); ++i) {
    topLine
      << setw(width+1) << "=";
  }  // -----  end for  ----- 
  topLine
    << setw(width+1)  << "=" // total column
    << setfill(' ')
    << endl;

  separatorLine 
    << " "
    << setfill('-')
    << setw(width+1)  << "+"
    << setw(width+1)  << "+";
  for (auto i = e.begin(); i != e.end(); ++i) {
  separatorLine
    << setw(width+1) << "+";
  }  // -----  end for  ----- 
  separatorLine
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
    << " Income" << " |";
  for (auto i = e.begin(); i != e.end(); ++i) {
    cout
    << " "  
    << setw(width-2) << setprecision(2) << fixed << left
    << (*i).name << " |";
  }  // -----  end for  ----- 
  cout 
    << setw(width-1) << fixed << left
    << " Total" << " |" 
    << std::endl
    << topLine.str();

  cout 
    << " "
    << right
    << setw(width-1)
    << "Name/Costs" << " |" 
    << setw(width-1) << setprecision(2) << fixed << right
    << " " <<  " |";
  double sumCosts =0.;
  for (auto i = e.begin(); i != e.end(); ++i) {
    cout
      << setw(width-1) << setprecision(2) << fixed << right
      << (*i).cost << " |";
      sumCosts += (*i).cost;
  }  // -----  end for  ----- 
  cout 
    << setw(width-1) << setprecision(2) << fixed << right
    << sumCosts << " |" 
    << std::endl
    << separatorLine.str();

  for (auto j = p.begin(); j != p.end(); ++j) {
    auto final_j = p.end();
    --final_j;
    cout 
      << " "
      << right
      << setw(width-1)
      << (*j).name << " |" 
      << setw(width-1) << setprecision(2) << fixed << right
      << (*j).income << " |";

    for (auto i = e.begin(); i != e.end(); ++i) {
      cout 
        << setw(width-1) << setprecision(2) << fixed << right
        << (*j).income * ratio * (*i).cost << " |";
    }  // -----  end for expenses  ----- 
    cout 
      << setw(width-1) << setprecision(2) << fixed << right
      << (*j).income * ratio * sumCosts << " |" 
      << std::endl;
      if (j != final_j) {
        cout << separatorLine.str();
      }
  }  // -----  end for persons  ----- 

  double percentTotal = sumCosts*ratio;
  std::cout
    << std::endl
    << std::endl
    <<"Every person pays a fair share of "
    << bold << 100*percentTotal <<"%" << normal
    <<" of her/his income."
    << std::endl;
}  // -----  end of function DisplayResults  -----
