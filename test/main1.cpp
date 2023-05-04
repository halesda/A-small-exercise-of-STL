#include<iostream>
#include"../graph.h"
using namespace std;

int main()
{


	graph_2 g(9);

	g.set(0,5,4);
	g.set(0,1,3);
	g.set(1,6,6);
	g.set(1,8,5);
	g.set(1,2,8);
	g.set(2,8,2);
	g.set(2,3,12);
	g.set(3,6,14);
	g.set(3,7,6);
	g.set(3,4,10);
	g.set(3,8,11);
	g.set(4,7,1);
	g.set(4,5,18);
	g.set(5,6,7);
	g.set(6,7,9);
	
	g.prim();
	g.kruskal();

	return 0;
}

