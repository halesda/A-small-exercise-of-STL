#include<iostream>
using namespace std;
#include"../vector.h"
#include"class.h"

int main()
{
	my_stl::vector<test> v;
	for(int i = 0;i < 10;++i)
	{
		v.push_back(test(i));
	}
	for(int i = 0;i < 10;++i)
	{
		v[i] = test(i+ 10);
	}

	return 0;
}
