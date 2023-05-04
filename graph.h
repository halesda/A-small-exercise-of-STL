#ifndef _GRAPH_H_
#define _GRAPH_H_

#include<cassert>
#include<stack>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

//图的邻接矩阵存储结构，可以存储有向图，无向图，带权图，无权图
class graph_1
{
public:

	//初始化图
	graph_1(int n) : _n(n)
	{
		assert(n > 0);
		_edges = new int* [n];
		for(int i = 0;i < n;++i)
		{
			_edges[i] = new int[n];
		}

		is_visited = new bool[n];

		for(int i = 0;i < n;++i)
		{
			for(int j = 0;j < n;++j)
			{
				_edges[i][j] = -1;
			}
			is_visited[i] = false;
		}
	}

	graph_1(const graph_1& rhs) = delete;

	graph_1& operator= (const graph_1& rhs) = delete;

	~graph_1() noexcept
	{
		if(_edges)
		{
			for(int i = 0;i < _n;++i)
			{
				delete [] _edges[i];
			}
			delete [] _edges;
		}
		if(is_visited)
		{
			delete [] is_visited;
		}
	}

	//设置图的一条特定边的权值
	void set(int v1,int v2,int weight) noexcept
	{
		_edges[v1][v2] = weight;
	}

	//非递归的BFS
	void BFS(int v) noexcept
	{
		assert(v < _n);
		_clear();
		std::queue<int> q;
		q.push(v);
		while(!q.empty())
		{
			auto i = q.front();
			q.pop();
			cout << i << "  ";
			is_visited[i] = true;
			for(int j = 0;j < _n;++j)
			{
				if(_edges[i][j] != -1 && !is_visited[j])
				{
					q.push(j);
				}
			}
		}
		std::cout << std::endl;
		_clear();
	}

	//非递归的DFS
	void DFS_1(int v) noexcept
	{
		assert(v < _n);
		std::stack<int> s;
		s.push(v);
		_clear();
		while(!s.empty())
		{
			auto i = s.top();
			s.pop();
			cout << i << "  ";
			is_visited[i] = true;
			for(int j = 0;j < _n;++j)
			{
				if(_edges[i][j] != -1 && !is_visited[j])
				{
					s.push(j);
					is_visited[j] = true;
				}
			}
		}
		cout << endl;
	}

	//递归的DFS
	void DFS_2(int v) noexcept
	{
		assert(v < _n);
		_clear();
		_DFS_2(v);
		_clear();
		cout << endl;
	}


protected:

	void _clear()
	{
		for(int i = 0;i < _n;++i)
		{
			is_visited[i] = false;
		}
	}

	void _DFS_2(int v) noexcept
	{
		if(!is_visited[v])
		{
			cout << v << "  ";
			is_visited[v] = true;
		}
		if(std::all_of(is_visited,is_visited + _n,[](bool v){return v == true;}))
		{
			return;
		}
		for(int i = _n - 1;i >= 0;--i)
		{
			if(_edges[v][i] != -1 && !is_visited[i])
			{
				_DFS_2(i);
			}
		}
	}

private:
	int** _edges;  //存储边的权值
	const int _n;   
	bool* is_visited;  //标志访问数组
};


//并查集
class Union_Find_Disjoint_Sets
{
public:

        //并查集初始化操作，默认初始时所有的结点的根节点就是自身
        Union_Find_Disjoint_Sets(size_t n)
        {
                _set.resize(n + 1, 0);
                for (size_t i = 0; i < n + 1; ++i)
                {
                        _set[i] = i;
                }
        }


        ~Union_Find_Disjoint_Sets()noexcept {}


        //把两个结点i和j合并成为同一个结点
        void merge(size_t i, size_t j)
        {
                size_t a = _find_root(i);    //先找到i的根节点，得到根节点的编号
                size_t b = _find_root(j);    //然后在找到j的根节点，得到根节点的编号
                _set[a] = b;                        //连接i和j
                _set[i] = b;                         //路径压缩
        }

        //检测根节点i和j是不是在同一个关系中，也就是怕段它们是否有相同的根节点
        bool test(size_t i, size_t j)
        {
                size_t a = _find_root(i);
                size_t b = _find_root(j);
                return a == b;
        }


protected:

        //找一个结点的根结点，得到根节点的编号
        size_t _find_root(size_t x)
        {
                assert(x >= 0 && x < _set.size());
                if (_set[x] == x)     //这个情况是根节点就是自身
                {
                        return _set[x];
                }
                return _set[x] = _find_root(_set[x]);   //这个情况是根节点不是自身，那就继续递归，同时做一个路径压缩操作
        }

private:

        //并查集数组
        vector<size_t> _set;
};



//图的邻接表存储结构,用来存储无向带权图
//可以较好的写出最小生成树的算法
struct vertex
{
	int number; //顶点编号
};

struct edge
{
	int begin;  //边开始顶点
	int end;   // 边结束顶点
	int weight;  //权值
};

struct comp
{
	bool operator()(edge* l,edge* r) const
	{
		return l->weight > r->weight;
	}
};

class graph_2
{
public:

	graph_2(int n) : _n(n)
	{
		assert(_n > 0);
		for(int i = 0;i < n;++i)
		{
			_vertexs.push_back(new vertex());
			_vertexs.back()->number = i;
		}
	}

	~graph_2() noexcept
	{
		if(!_edges.empty())
		{
			for(auto c : _edges)
			{
				delete c;
			}
		}
		if(!_vertexs.empty())
		{
			for(auto c : _vertexs)
			{
				delete c;
			}
		}
	}

	void set(int v1,int v2,int weight) noexcept
	{
		assert(v1 < _n && v2 < _n);
		_edges.push_back(new edge());
		_edges.back()->begin = v1;
		_edges.back()->end = v2;
		_edges.back()->weight = weight;
	}

	//prim算法求无向带权图的最小生成树
	void prim() noexcept
	{
		vector<vertex*> v_new;
		vector<edge*> e_new;
		v_new.push_back(_vertexs[rand() % _vertexs.size()]);
		sort(_edges.begin(),_edges.end(),[](edge* l,edge* r){return l->weight < r->weight;});

		
		while(v_new.size() != _vertexs.size())
		{
			for(auto c : _edges)
			{
				auto iter1 = find_if(v_new.begin(),v_new.end(),[&](auto x){return x->number == c->begin; });
				auto iter2 = find_if(v_new.begin(),v_new.end(),[&](auto x){return x->number == c->end; });
				auto iter3 = find_if(_vertexs.begin(),_vertexs.end(),[&](auto x){return x->number == c->end; });
				if(iter1 != v_new.end() && iter2 == v_new.end() && iter3 != _vertexs.end())
				{
					v_new.push_back(*iter3);
					e_new.push_back(c);
					break;
				}

				iter1 = find_if(v_new.begin(),v_new.end(),[&](auto x){return x->number == c->end; });
				iter2 = find_if(v_new.begin(),v_new.end(),[&](auto x){return x->number == c->begin; });
				iter3 = find_if(_vertexs.begin(),_vertexs.end(),[&](auto x){return x->number == c->begin; });
				if(iter1 != v_new.end() && iter2 == v_new.end() && iter3 != _vertexs.end())
				{
					v_new.push_back(*iter3);
					e_new.push_back(c);
					break;
				}
			}
		}

		for(auto c : e_new)
		{
			cout << c->weight << "  ";
		}
		cout << endl;
	}

	//kruskal算法求无向带权图的最小生成树
	void kruskal() noexcept
	{
		priority_queue<edge*,vector<edge*>,comp> q;
		for(auto c : _edges)
		{
			q.push(c);
		}
		vector<edge*> e_new;
		Union_Find_Disjoint_Sets s(_vertexs.size());
		while(!q.empty())
		{
			auto i = q.top();
			q.pop();
			if(!s.test(i->begin,i->end))
			{
				s.merge(i->begin,i->end);
				e_new.push_back(i);
			}
		}
		for(auto c : e_new)
		{
			cout << c->weight << "  ";
		}
		cout << endl;
	}

private:
	vector<edge*> _edges;  //边集合
	vector<vertex*> _vertexs;  //顶点集合
	const int _n;
};

#endif
