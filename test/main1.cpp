#include<iostream>
using namespace std;
#include"../vector.h"

int main()
{
	my_stl::vector<int> v;
	for(int i = 0;i < 10;++i)
	{
		v.push_back(i);
	}

	return 0;
}
