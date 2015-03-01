//
// =========================================================================
//
//       Filename:  helper-functions.cc
// 
//    Description:  Definition of general helper functions for pretty
//                  output, conversion, file handling etc.
// 
//        Version:  1.0
//        Created:  02/04/2014 09:02:28 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Dr. Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  Technical University Berlin
//
// =========================================================================
//

#include <iostream>  // input/output streams, e.g. cout and cin
#include <fstream>   // filestreams to read and write data to files
#include <vector>    // vector handling
#include <string>    // string handling
#include <sstream>   // string streams to join different strings
#include <cfloat>    // convert strings to doubles

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "global-constants.h"
#include "helper-functions.h"

using namespace std;

// conversion
double StringToDouble(const string & doubleString) {
  char const *s = doubleString.c_str();
  char *end;
  errno = 0;

  double d = strtod(s, &end);

  if ((errno == ERANGE && d <= DBL_MAX && d >= DBL_MAX) || d > DBL_MAX) {
    DisplayError("Overflow in string to double conversion.");
    exit(EXIT_FAILURE);
  }

  if ((errno == ERANGE && d <= DBL_MIN && d >= DBL_MIN) || d < DBL_MIN) {
    DisplayError("Underflow in string to double conversion of " + doubleString);
    
    exit(EXIT_FAILURE);
  }

  if (*s == '\0' || *end != '\0') {
    DisplayError("Inconvertible in string to double conversion.");
    exit(EXIT_FAILURE);
  }

  return d;
}       // ----------  end of function StringT0Double  ----------

// number treatment
bool IsNumber(const double x) {
  // comparing two doubles with == typically gives a compiler error but in
  // this case the comparison is ok and thus the waring can be ignored
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
  // This looks like it should always be true,
  // but it's false if x is a NaN.
  return (x == x);
#pragma GCC diagnostic pop
}

bool IsFiniteNumber(const double x) {
  return (x <= DBL_MAX && x >= -DBL_MAX); 
} 

// output
void DisplayError (const string & errorMessage) {
  cerr << endl << endl;
  cerr << red << "ERROR: " << normal << errorMessage; 
  cerr << endl << endl;
}   // -----  end of function DisplayError  -----

void DisplayFix (const string & fixMessage) {
  cout << green << "FIX:   " << normal << fixMessage; 
  cout << endl ;
}   // -----  end of function DisplayError  -----

void SetStreamFormatToScientificAndGivenPrecision(std::ostream & os, int precision) {
    os.setf(ios_base::scientific, ios_base::floatfield);
    os.precision(precision);
}   // -----  end of function AlterStreamOutputFormatToScientificAndPrecisionOf10  -----

void SetStreamFormatToFixedAndGivenPrecision(std::ostream & os, int precision) {
    os.setf(ios_base::fixed, ios_base::floatfield);
    os.precision(precision);
}   // -----  end of function AlterStreamOutputFormatToScientificAndPrecisionOf10  -----

// file handling
void OpenNewFileToWriteOrExit(const string & fileName, ofstream & ofs) {
  CheckFileExistsOrExit(fileName);
  ofs.open(fileName,ios::out);
}   // -----  end of function OpenNewFileToWriteOrExit -----

void OpenExistingFileToAppendOrExit(const string & fileName, ofstream & ofs) {
  CheckFileExistsOrExit(fileName);
  ofs.open(fileName,ios::app);
}   // -----  end of function OpenNewFileToAppendOrExit -----

void OpenFileToReadOrExit(const string & fileName, ifstream & ifs) {
  CheckFileExistsOrExit(fileName);
  ifs.open(fileName,ios::in);
}   // -----  end of function OpenFileToReadOrExit -----

bool FileExists(const string & fileName) {
  bool fileIsOpenToRead  = false;
  ifstream ifs(fileName.c_str());
  fileIsOpenToRead  = ifs.is_open();
  ifs.close();
  return fileIsOpenToRead;
}   // -----  end of function FileExists  -----

void CheckFileExistsOrExit(const string & fileName) {
  if(!FileExists(fileName)){
    DisplayError("Could not open file " + fileName );
    std::cout <<  "Current path is: ";
    int returnValue = system("pwd");
    if (returnValue != 0) {
      DisplayError("System command 'pwd' returned with status " + returnValue);
      exit(returnValue);
    }
    exit(EXIT_FAILURE);
  }
}   // -----  end of function CheckFileExistsOrExit  -----

int CountCommentLinesOfFile(const string & fileName){

  ifstream(ifs);
  OpenFileToReadOrExit(fileName,ifs);

  int counterCommentLines = CountCommentLinesOfStream(ifs);

  ifs.close();
  return counterCommentLines;
}   // -----  end of function CountCommentLinesOfFile  -----

int CountCommentLinesOfStream(ifstream & ifs) {
  string firstStringOfLine ="";
  int counterCommentLines = 0;

  ifs >> firstStringOfLine;
  char firstCharOfString = firstStringOfLine.c_str()[0];

  while (firstCharOfString == '#' || firstCharOfString == '/'){
    ifs.ignore(300,'\n');
    ifs >> firstStringOfLine;
    firstCharOfString = firstStringOfLine.c_str()[0];
    counterCommentLines++;
  }       //----  end while  -----

  ifs.seekg(0); // rewind 

  return counterCommentLines;
}   // -----  end of function CountCommentLinesOfStream  -----

int CountLines(const string & fileName) {
  ifstream(ifs);
  OpenFileToReadOrExit(fileName,ifs);
  // new lines will be skipped unless we stop it from happening:    
  ifs.unsetf(std::ios_base::skipws);

  // count the newlines with an algorithm specialized for counting:
  int lineCount = static_cast<int>(
                    std::count(
                      std::istream_iterator<char>(ifs),
                      std::istream_iterator<char>(), 
                      '\n'
                      )
                  );
  return lineCount;
}   // -----  end of function CountLines  -----

void IgnoreCommentLinesAtBeginningOfFile(ifstream & ifs) {
  int counterCommentLines = CountCommentLinesOfStream(ifs);

  for (int i = 0; i < counterCommentLines; ++i) {
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }       //----  end for  -----
}   // -----  end of function IgnoreCommentLinesAtBeginningOfFile  -----

void IgnoreTillEndOfLineInStream(ifstream & ifs) {
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}   // -----  end of function IgnoreTillEndOfLineInStream  -----
