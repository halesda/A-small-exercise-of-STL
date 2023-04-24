#include<iostream>
using namespace std;
#include"../vector.h"
#include"class.h"
#include"../set.h"

int main()
{
	my_stl::set<test> s1,s2;
	cout << (s1 == s2) << endl;
	s1.insert(test(10));
	s2[test(10)] = test(10);
	cout << (s1 == s2) << endl;



	return 0;
}
