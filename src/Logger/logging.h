#ifndef LOGGING_H_
#define LOGGING_H_

#include <string>
#include <iostream>
#include "../Flags/gflags.h"

using namespace std;

enum Log_Level {
  DEBUG,
  ERROR,
  INFO
};

/*
 * The null_stream class sends incoming log messages into the vast nothingness
 */
class Null_Stream : public ostream {
  public:
    // constructor to enable constructing the class without parameters
    Null_Stream():ostream(NULL) {};
    
    // override of operator << to ignore the parameter
    template<typename T> 
    ostream& operator<< (const T& t) {
      return *this;
    }
};

// initalize the logging system: open any log files, etc ...
void init_logging_system();

// returns an appropriate ostream for logging
ostream & log(Log_Level level, bool newline = true);

#endif /*LOGGING_H_*/
