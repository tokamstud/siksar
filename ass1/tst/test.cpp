#include <iostream>
#include <vector>
using namespace std;

// main() is where program execution begins.

int main()
{
  int myints[] = {10,20,30,5,15};
  vector<int> v(myints,myints+5);

  cout << myints << endl;
  cout << myints+5 << endl;
  return 0;
}
