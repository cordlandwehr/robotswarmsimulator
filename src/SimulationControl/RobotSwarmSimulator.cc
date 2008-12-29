//============================================================================
// Name        : RobotSwarmSimulator.cpp
// Author      : Christoph Raupach
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/lambda/lambda.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>

#include "../testheader.h"
#include "../testheaderB.h"

using namespace std;


int main(int argc, char *argv[]) {
  using namespace boost::lambda;

  Test test;
  cout << "testing " << test.correct_add(2,2) << endl ;

  TestB testb(test);
  cout << "testing 2 " << testb.correct_add(2,2) << endl;

  boost::scoped_ptr<Test> smart_pointer;
  smart_pointer.reset(new Test());
  cout << "Testing smart pointer " << smart_pointer->correct_add(2,3) << endl;

  std::string line;
  boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

  cout << "Main Program";
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

