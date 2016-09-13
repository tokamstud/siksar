// clearing vectors
#include <iostream>
#include <vector>

int main ()
{
  std::vector<char> myvector;
  myvector.push_back('1');
  myvector.push_back('2');
  myvector.push_back('3');

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  myvector.clear();

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}
