#include "logging.h"
#include <fstream>

DEFINE_bool(debug, false, "log debug messages");
DEFINE_bool(logonlyerrors, false, "log only error messages");
DEFINE_bool(logtofile, false, "log to a file");
DEFINE_string(logfile, "log.txt", "default filename of log");

Null_Stream global_null_stream;
fstream log_file_stream(NULL, fstream::out);

void init_logging_system() {
  if (FLAGS_logtofile) {
    log_file_stream.close();
    log_file_stream.open(FLAGS_logfile.c_str(), fstream::out);
  }
}

ostream & log(Log_Level level, bool newline) {
  // check if level is sufficient to log
  if ((level == DEBUG && !FLAGS_debug) ||
       level == INFO && FLAGS_logonlyerrors) {
    return global_null_stream;
  }
  
  // build the output stream (either to stdout or to a file)
  ostream * output_stream;
  if (FLAGS_logtofile) {
    output_stream = &log_file_stream;
  } else {
    output_stream = &cout;
  }
  
  // start a new line and log the level of the message
  if (newline) {
    *output_stream << endl;
  
    if (level == DEBUG) {
      *output_stream << "DEBUG: ";
    } else if (level == INFO) {
      *output_stream << "INFO: ";
    } else if (level == ERROR) {
      *output_stream << "ERROR: ";
    }
  }
  return *output_stream;
}