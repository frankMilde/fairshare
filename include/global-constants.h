//
// =========================================================================
//
//       Filename:  global-constants.h
// 
//    Description:  Declares program specific global constants.
// 
//        Version:  1.0
//        Created:  02/05/2014 11:32:58 AM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dr. Frank Milde (FM), frank.milde (at) posteo.de
//        Company:  Technical University Berlin
//
// =========================================================================
//
#ifndef  GLOBAL_CONSTANTS_INC
#define  GLOBAL_CONSTANTS_INC

//--------------------------------------------------------------------------
//  enumerates
//--------------------------------------------------------------------------
enum Error : unsigned short {
  Success,
  Failure,
};        // ----------  end of enum Error  ----------

//--------------------------------------------------------------------------
//  constants
//--------------------------------------------------------------------------
// ASCII color codes {{{
// color types //{
// To test colors in terminal run the following loop in main()
//    for (int i = 1; i <= 107; i++){
//      std::cout << i <<  " \033[0;" << i << "mHello!\033[0m" << std::endl;
//      std::cout << i <<  " \033[1;" << i << "mHello!\033[0m" << std::endl;
//    }
//}
extern const std::string black; 
extern const std::string gray; 
extern const std::string red; 
extern const std::string green; 
extern const std::string yellow; 
extern const std::string blue; 
extern const std::string violet; 
extern const std::string cyan; 
extern const std::string bold; 
extern const std::string underline; 
extern const std::string normal; 

// file names {{{
extern const std::string kIniFileName;
//}}}
#endif   //---- #ifndef GLOBAL_CONSTANTS_INC  -----
