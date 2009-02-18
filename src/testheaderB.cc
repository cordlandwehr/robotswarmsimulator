#include "testheaderB.h"

TestB::TestB(Test test) {
  test_ = test;
}

int TestB::correct_add(int a, int b) {
  return test_.correct_add(a,b);
}
