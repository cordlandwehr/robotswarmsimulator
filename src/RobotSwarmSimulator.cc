//============================================================================
// Name        : RobotSwarmSimulator.cpp
// Author      : Christoph Raupach
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "testheader.h"

using namespace std;

int main(int argc, char *argv[]) {
  using namespace boost::lambda;
  typedef std::istream_iterator<int> in;
  cout << "please enter some numbers" ;
  std::for_each(
      in(std::cin), in(), std::cout << (_1 * 3) << " " );

	return 0;
}
