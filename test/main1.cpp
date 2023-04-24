#include<iostream>
#include"class.h"
#include"../set.h"
#include"../map.h"
using namespace std;

int main()
{

	my_stl::set<test> t1;
	t1[test(10)] = test(20);
	cout << t1[test(10)] << endl;

	return 0;
}
