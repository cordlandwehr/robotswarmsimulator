#ifndef UNSUPPORTED_OPERATION_EXCEPTION_H_
#define UNSUPPORTED_OPERATION_EXCEPTION_H_

#include <stdexcept>
#include <string>
#include "console_output.h"

class UnsupportedOperationException : public std::runtime_error {
public:
	UnsupportedOperationException(const std::string& msg) : std::runtime_error(msg) {
		;
	}
	virtual ~UnsupportedOperationException() throw () {
		;
	}
};

#endif /* UNSUPPORTED_OPERATION_EXCEPTION_H_ */
