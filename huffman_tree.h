#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_
#include"priority_queue.h"
#include"unordered_map.h"
#include"string.h"
namespace my_stl
{

	//哈夫曼树节点结构体
	struct _huffman_tree_node_
	{
		size_t _n;
		char _s;
		_huffman_tree_node_* _left;
		_huffman_tree_node_* _right;
	};


	//节点比较规则
	struct compare
	{
		inline bool operator()(_huffman_tree_node_* left,_huffman_tree_node_* right) const
		{
			return left->_n > right->_n;
		}
	};


	//哈夫曼树结构体
	template<class Allocator = allocator<_huffman_tree_node_>>
		class huffman_tree
		{
			public:
				using _pnode = _huffman_tree_node_*;
				using _node = _huffman_tree_node_;


				//使用一个字符串初始化一颗哈夫曼树
				void init(const string& _str) noexcept
				{
					_init_(_str);
				}

				//清空这颗哈夫曼树
				void clear() noexcept
				{
					_clear_(_root);
					minimun_code = 0;
					_root = 0;
					_m1.clear();
					_m2.clear();
				}

				//对初始化的字符串进行编码
				string encode(const string& str) noexcept
				{
					string code;
					_encode_(_root,code);
					code.clear();
					for(auto i = str.begin();i != str.end();++i)
					{
						code += _m1[*i];
					}
					return code;
				}

				//对一个字符串进行解码
				string decode(const string& str) noexcept
				{
					string code,cur_code;
					for(auto i = str.begin();i != str.end();++i)
					{
						cur_code += *i;
						if(cur_code.size() >= minimun_code)
						{
							auto it = _m2.find(cur_code);
							if(it != _m2.end())
							{
								code += it->second;
								cur_code.clear();
							}
						}
					}
					return code;
				}

			protected:

				void _encode_(_pnode pt,string& code) noexcept
				{
					if(!pt->_left && !pt->_right)
					{
						print("char ",pt->_s,", counts = ",pt->_n,", code = ",code,"\n");
						_m1.insert(pt->_s,code);
						_m2.insert(code,pt->_s);
						auto siz = code.size();
						if(siz < minimun_code)
						{
							minimun_code = siz;
						}
					}
					if(pt->_right)
					{
						code += '1';
						_encode_(pt->_right,code);
						code.pop_back();
					}
					if(pt->_left)
					{
						code += '0';
						_encode_(pt->_left,code);
						code.pop_back();
					}
				}

				void _clear_(_pnode pt) noexcept
				{
					if(pt == nullptr)
					{
						return;
					}
					_clear_(pt->_left);
					_clear_(pt->_right);
					allo.deallocate(pt);
				}

				void _init_(const string& _str) noexcept
				{
					unordered_map<char,size_t> m;
					for(auto i = _str.begin();i != _str.end();++i)
					{
						++m[*i];
					}
					_pnode pt = nullptr;
					for(auto i = m.begin();i != m.end();++i)
					{
						pt = static_cast<_pnode>(allo.allocate(sizeof(_node)));
						allo.construct(&pt->_s,i->first);
						allo.construct(&pt->_n,i->second);
						_q.push(pt);
					}
					_pnode left = nullptr,right = nullptr;
					while(_q.size() > 1)
					{
						pt = static_cast<_pnode>(allo.allocate(sizeof(_node)));
						left = _q.top();
						_q.pop();
						right = _q.top();
						_q.pop();
						pt->_left = left;
						pt->_right = right;
						pt->_n = left->_n + right->_n;
						_q.push(pt);
					}
					_root = _q.top();
					_q.clear();
				}

			private:

				priority_queue<_pnode,compare,vector<_pnode>> _q; //使用一个优先级队列构建哈夫曼树
				size_t minimun_code = 114514;    //技巧
				unordered_map<char,string> _m1;   //保存编码和解码的字符串
				unordered_map<string,char> _m2;   //保存编码和解码的字符串
				_pnode _root;       //树根
				Allocator allo;      //空间适配器

		};
};
#endif
