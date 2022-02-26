#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector
static bool comp( int a, int b) {
    return (a > b);
}
void dump(std::vector<int>& v) {
  for (unsigned i=0; i<v.size(); i++)
    std::cout << ' ' << v[i];
  std::cout << '\n';
}

int main () {
  int myints[] = {10,20,30,5,15};
  std::vector<int> v(myints,myints+5);
  std::cout << "origin vector:";
  dump(v);
  std::make_heap (v.begin(),v.end(), comp);
  std::cout << "initial max heap:";
  dump(v);
  
  std::pop_heap (v.begin(),v.end(), comp); 
  v.pop_back();
  std::cout << "max heap after pop : ";
  dump(v);

  v.push_back(99); 
  std::push_heap (v.begin(),v.end(), comp);
  std::cout << "max heap after push 99 : ";
  dump(v);

  // std::sort_heap (v.begin(),v.end(), comp);

  // std::cout << "final sorted range :";

  return 0;
}
