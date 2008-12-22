//============================================================================
// Name        : RobotSwarmSimulator.cpp
// Author      : Christoph Raupach
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/lambda/lambda.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>

#include "testheader.h"

using namespace std;

int main(int argc, char *argv[]) {
  using namespace boost::lambda;
  
  std::string line;
  boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

  while (std::cin)
  {
      std::getline(std::cin, line);
      boost::smatch matches;
      if (boost::regex_match(line, matches, pat))
          std::cout << matches[2] << std::endl;
  }

  typedef std::istream_iterator<int> in;
  cout << "please enter some numbers" ;
  std::for_each(
      in(std::cin), in(), std::cout << (_1 * 3) << " " );

	return 0;
}
