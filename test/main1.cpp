#include<iostream>
#include"class.h"
#include"../set.h"
#include"../map.h"
using namespace std;

int main()
{

	my_stl::set<test> t1;
	my_stl::map<test,test> m1;
	for(int i = 0;i < 100;++i)
	{
		t1.insert(test(i));
		m1.insert(test(i),test(i));
	}
	auto i = t1.begin();
	auto j = m1.begin();
	for(;i != t1.end() && j != m1.end();++i,++j)
	{
		cout << i->first <<"  " << i->second << endl;
		cout << j->first <<"  " << j->second << endl;
	}

	return 0;
}
