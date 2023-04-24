#include<iostream>
#include"class.h"
#include"../vector.h"
using namespace my_stl;

int main()
{

	vector<test> t;
	for(int i = 10;i > 0;--i)
	{
		t.push_back(test(i));
	}
	for(auto i = t.begin();i != t.end();++i)
	{
		cout << *i << endl;
	}
	_insert_sort(t.begin(),t.end());
	random_shuffle(t.begin(),t.end());
	for(auto i = t.begin();i != t.end();++i)
	{
		cout << *i << endl;
	}


	return 0;
}
