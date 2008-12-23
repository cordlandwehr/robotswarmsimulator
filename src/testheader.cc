#include "testheader.h"
#include <iostream>
using namespace std;

void Test::say_hello() {
	cout << "testing include" << endl; 
}

int Test::correct_add(int a, int b) {
  return a + b;
}

int Test::incorrect_add(int a, int b) {
  return a + b + 1;
}