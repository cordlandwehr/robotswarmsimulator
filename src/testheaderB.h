#ifndef TESTHEADERB_H_
#define TESTHEADERB_H_

#include "testheader.h"

class TestB {
  public:
    TestB(Test test);
    int correct_add(int a, int b);
    
  private:
    Test test_;
};

#endif /*TESTHEADERB_H_*/
