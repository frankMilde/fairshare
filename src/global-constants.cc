//
// =========================================================================
//
//       Filename:  global-constants.cc
// 
//    Description:  Defines program specific global constants.
// 
//        Version:  1.0
//        Created:  02/20/2014 01:46:30 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dr. Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  Technical University Berlin
//
// =========================================================================

#include <string>

#include "global-constants.h"

// ASCII color codes {{{
// color types //{
// To test colors in terminal run the following loop in main()
//    for (int i = 1; i <= 107; i++){
//      std::cout << i <<  " \033[0;" << i << "mHello!\033[0m" << std::endl;
//      std::cout << i <<  " \033[1;" << i << "mHello!\033[0m" << std::endl;
//    }
//}
const std::string black     = "\033[0;30m";
const std::string gray      = "\033[1;30m";
const std::string red       = "\033[1;31m";
const std::string green     = "\033[1;32m";
const std::string yellow    = "\033[1;33m";
const std::string blue      = "\033[1;34m";
const std::string violet    = "\033[1;35m";
const std::string cyan      = "\033[1;36m";
const std::string bold      = "\033[1m";
const std::string underline = "\033[4m";
const std::string normal    = "\033[0m";
//}}}

// file names {{{
const std::string kIniFileName = "settings.ini";
//}}}
