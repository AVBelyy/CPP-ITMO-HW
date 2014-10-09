// using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

int b;
int * const a = &b;

class myexception: public exception
{
  int how;
  public:
  myexception(int how) : how(how)
  {
  }

  virtual const char* what() const throw()
  {
    return "ololo";
  }
};

int main () {
  try
  {
    throw myexception(100500);
  }
  catch (exception& e)
  {
    cout << e.what() << '\n';
  }
  return 0;
}
