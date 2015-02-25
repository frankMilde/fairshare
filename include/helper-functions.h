//
// =========================================================================
//
//       Filename:  helper-functions.h
// 
//    Description:  Declaration of general helper functions for pretty
//                  output, conversion, file handling etc.
// 
//        Version:  1.0
//        Created:  02/04/2014 09:25:42 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dr. Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  Technical University Berlin
//
// =========================================================================
//

#ifndef  HELPER_FUNCTIONS_INC
#define  HELPER_FUNCTIONS_INC

bool FileExists                    (const std::string   & fileName );
void CheckFileExistsOrExit         (const std::string   & fileName );
void OpenNewFileToWriteOrExit      (const std::string   & fileName,
                                          std::ofstream & ofs      );
void OpenFileToReadOrExit          (const std::string   & fileName, 
                                          std::ifstream & ifs      );
void OpenExistingFileToAppendOrExit(const std::string   & fileName,
                                          std::ofstream & ofs      );

int  CountLines                    (const std::string   & fileName );
int  CountCommentLinesOfFile       (const std::string   & fileName );
int  CountCommentLinesOfStream     (      std::ifstream & ifs      );
void IgnoreTillEndOfLineInStream   (      std::ifstream & ifs      );
void IgnoreCommentLinesAtBeginningOfFile( std::ifstream & ifs      );

void DisplayError                  (const std::string & errorMessage);
void DisplayFix                    (const std::string &   fixMessage);

void SetStreamFormatToScientificAndGivenPrecision(
    std::ostream & ofs, int precision);
void SetStreamFormatToFixedAndGivenPrecision(
    std::ostream & ofs, int precision);


template<typename T>
void        DisplayVector(const std::vector<T> & v) {
  typename std::vector<T>::const_iterator citer;
  
  std::cout << std::endl << std::endl;

  for (citer = v.begin(); citer != v.end(); citer++) {
    SetStreamFormatToScientificAndGivenPrecision(std::cout,10);
    std::cout << *citer << std::endl;
  }

  std::cout << std::endl;
}   // -----  end of function DisplayVector  -----

bool        IsNumber       (const double x);
bool        IsFiniteNumber (const double x);

double      StringToDouble (const std::string & doubleAsString);

template <typename T>
std::string NumberToString (const T & number){
  std::ostringstream convert; // stream used for the conversion

  convert << number;          // insert the textual representation of
                              // 'Number' in the characters in the stream
  return convert.str(); 
}
// }}}
#endif   //---- #ifndef HELPER_FUNCTIONS_INC  -----
