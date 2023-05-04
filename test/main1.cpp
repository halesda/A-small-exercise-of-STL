#include<iostream>
#include"../algorithm.h"
#include<algorithm>
#include<vector>
using namespace std;

int main()
{

	srand(time(0));
	int array[10000];
	for(int i = 0;i < 10000;++i)
	{
		array[i] = rand() % 100000;
	}
	vector<int> v(array,array + my_stl::array_size(array));
	my_stl::select_sort(array,array + my_stl::array_size(array));
	sort(v.begin(),v.end());

	for(int i = 0;i < 10000;++i)
	{
		if(v[i] != array[i])
		{
			cout << "false" << endl;
		}
	}
	cout << "true" << endl;



	return 0;
}

