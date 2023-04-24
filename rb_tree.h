#ifndef _RB_TREE_H_
#define _RB_TREE_H_
#include"allocator.h"
#include"iterator_traits.h"
#include"type_traits.h"
#include"memory.h"
#include"algorithm.h"
#include"functional.h"

/*
Red-black tree（红黑树）class，用来当做SLT关联容器的底层机制（如set，multiset，map，
multimap）

//红黑树的实现有两个技巧：
(1)header不仅指向root，也指向红黑树的最左节点，以便用常数时间实现begin()，并且也指向红黑树的最右边节点，以便
set相关泛型算法（如set_union等等）可以有线性时间实现。
(2)当一个即将被删除的节点有两个孩子节点时，它的successor（后继）node is relinked into its place, ranther than copied,
如此一来唯一失效的（invalidated）的迭代器就只是那些referring to the deleted node.
*/


//类声明
template<class Key, class Value,class Comp,class Alloc>
class rb_tree;

template <class Key, class Value,class Comp>
class _rb_tree_iterator;


template <class Key, class Value>
struct _rb_tree_node;


//红色为0，黑色为1
static constexpr bool red = false;
static constexpr bool black = true;


//红黑树节点类型
template<class Key, class Value>
struct _rb_tree_node
{
	using key_type = Key;
	using mapped_type = Value;

	bool _color;   // 节点颜色，红或者黑
	_rb_tree_node<Key, Value>* _parent; // RB树的许多操作，必须知道其父结点
	_rb_tree_node<Key, Value>* _left; // 指向左孩子节点
	_rb_tree_node<Key, Value>* _right;  // 指向右孩子节点
	pair<Key, Value> _pair;   //存放键值对
};


//红黑树迭代器
template<class Key, class Value,class Comp>
struct _rb_tree_iterator
{	
public:

		//内嵌类型声明，为了萃取和类型安全
		friend class rb_tree<Key, Value,Comp,allocator<pair<Key,Value>>>;
		using _pnode_ = _rb_tree_node<Key, Value>*;
		using _node_ = _rb_tree_node<Key,Value>;
		using value_type = pair<Key,Value>;
		using iterator_category = bidirectional_iterator_tag;
		using reference = value_type&;
		using pointer = value_type*;
		using distance_type = ptrdiff_t;
		using size_type = size_t;
		static_assert (is_same_v<Key,typename _node_::key_type>);
		static_assert (is_same_v<Value,typename _node_::mapped_type>);

		//析构函数什么也不干
		inline ~_rb_tree_iterator()noexcept {}

		//构造函数，使用一个红黑树节点指针构造
		inline _rb_tree_iterator(_pnode_ pt = nullptr)noexcept :_node(pt) {}

		//拷贝构造
		inline _rb_tree_iterator(const _rb_tree_iterator& rhs)noexcept :_node(rhs._node) {}

		//移动构造
		inline _rb_tree_iterator(_rb_tree_iterator&& rhs) noexcept : _node(rhs._node)
		{
			rhs._node = nullptr;
		}

		//移动赋值
		inline _rb_tree_iterator& operator=(_rb_tree_iterator&& rhs) noexcept
		{
			if(this != &rhs)
			{
				_node = rhs._node;
				rhs._node = nullptr;
			}
			return *this;
		}

		//拷贝赋值
		inline _rb_tree_iterator& operator=(const _rb_tree_iterator& rhs) noexcept
		{
			if (this != &rhs)
			{
				_node = rhs._node;
			}
			return *this;
		}

		//解引用操作符，返回pair引用
		inline pair<Key, Value>& operator*() noexcept
		{
			assert(_node);
			return _node->_pair;
		}

		//解引用操作符，返回pair引用
		inline pair<Key, Value>& operator*() const noexcept
		{
			assert(_node);
			return _node->_pair;
		}

		//指针操作符，返回pair地址
		inline pair<Key, Value>* operator->()const noexcept
		{
			assert(_node);
			return &(_node->_pair);
		}

		//指针操作符，返回pair地址
		inline pair<Key, Value>* operator->() noexcept
		{
			assert(_node);
			return &(_node->_pair);
		}

		//迭代器++
		inline _rb_tree_iterator& operator++()noexcept
		{
			//如果有右子树，那么就是右子树的最左边的节点，即最大节点
			if (_node->_right)
			{
				_node = _node->_right;
				while (_node->_left)
				{
					_node = _node->_left;
				}
			}
			else
			{
				_pnode_ y = _node->_parent;
				while (_node == y->_right)
				{
					_node = y;
					y = y->_parent;
				}
				if (_node->_right != y)
				{
					_node = y;
				}
			}
			return *this;
		}

		//迭代器++
		inline const _rb_tree_iterator operator++(int) noexcept
		{
			_rb_tree_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		//迭代器--
		inline _rb_tree_iterator& operator--()noexcept
		{
			if (_node->_color == red && _node->_parent->_parent == _node)
			{
				_node = _node->_right;
			}
			else if (_node->_left)
			{
				_pnode_ y = _node->_left;
				while (y->_right)
				{
					y = y->_right;
				}
				_node = y;
			}
			else
			{
				_pnode_ y = _node->_parent;
				while (_node == y->_left)
				{
					_node = y;
					y = y->_parent;
				}
				_node = y;
			}
			return *this;
		}

		//迭代器--
		inline const _rb_tree_iterator operator--(int) noexcept
		{
			_rb_tree_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		//判断两个迭代器是否相等
		inline bool operator==(const _rb_tree_iterator& rhs)const noexcept
		{
			return _node == rhs._node;
		}

		//判断两个迭代器是否不相等
		inline bool operator!=(const _rb_tree_iterator& rhs)const noexcept
		{
			return _node != rhs._node;
		}

	private:

		_pnode_ _node;  //实现迭代器维护一个红黑树节点指针
};


//红黑树类型
template<class Key, class Value,class Comp = less<Key>,class Alloc = allocator<pair<Key,Value>>>
class rb_tree
{
public:

		//内嵌类型声明，为了萃取和类型安全
		using iterator = _rb_tree_iterator<Key,Value,Comp>;
		using value_type = pair<Key,Value>;
		using pointer = value_type*;
		using reference = value_type&;
		using distance_type = ptrdiff_t;
		using size_type = size_t;
		using key_type = Key;
		using mapped_type = Value;
		static_assert(!is_const_v<Value>);
		static_assert(!is_void_v<Value>);
		static_assert(!is_const_v<Key>);
		static_assert(!is_void_v<Key>);
		static_assert(is_same_v<value_type,typename iterator::value_type>);
		static_assert(is_same_v<value_type,typename Alloc::value_type>);
		static_assert(is_same_v<value_type,typename iterator::value_type>);

		//默认构造函数,构造一棵空树
		inline rb_tree() noexcept:_n(0),_header_(__allocate_node_())
		{
			_header_->_parent = nullptr;
			_header_->_left = _header_;
			_header_->_right = _header_;
			_header_->_color = red;
		}

		//拷贝构造函数
		inline rb_tree(const rb_tree& rhs) noexcept : _n(0),_header_(__allocate_node_())			
		{
			_header_->_parent = nullptr;
			_header_->_left = _header_;
			_header_->_right = _header_;
			_header_->_color = red;
			for(auto i = rhs.begin();i != rhs.end();++i)
			{
				insert(i->first,i->second);
			}
		}

		//拷贝赋值函数
		rb_tree& operator= (const rb_tree& rhs) noexcept
		{
			if(this != &rhs)
			{
				clear();
				for(auto i = rhs.begin();i != rhs.end();++i)
				{
					insert(i->first,i->second);
				}
			}
			return *this;
		}

		//析构函数
		inline ~rb_tree()noexcept
		{
			clear();
			_allo.deallocate(_header_);
		}

		//返回首迭代器
		inline iterator begin()const noexcept
		{
			return iterator(_header_->_left);
		}

		//返回尾迭代器
		inline iterator end()const noexcept
		{
			return iterator(_header_);
		}

		//判断树是否为空
		inline bool empty()const noexcept
		{
			return _n == 0;
		}

		//获取树节点数量
		inline size_t size() const noexcept
		{
			return _n;
		}

		//获取键值最大的节点
		inline iterator max() const noexcept
		{
			_pnode_ x = _header_->_parent;
			if(x)
			{
				x = _maximum(x);
			}
			return iterator(x);
		}

		//获取键值最小的节点
		inline iterator min() const noexcept
		{
			_pnode_ x = _header_->_parent;
			if (x)
			{
				x = _minimum(x);
			}
			return iterator(x);
		}

		// 清空整颗树
		inline void clear() noexcept
		{
			_clear(_header_->_parent);
			_header_->_parent = nullptr;
			_header_->_left = _header_;
			_header_->_right = _header_;
			_n = 0;
		}

		//插入一个节点
		inline iterator insert(const Key& k,const Value& v) noexcept
		{
			return insert(pair<Key,Value>(k,v));
		}

		//插入一个节点
		iterator insert(const pair<Key, Value>& v) noexcept
		{
			_pnode_ y = _header_;
			_pnode_ x = _header_->_parent; 
			bool flag = true;
			while (x != nullptr)                //从根节点开始往下找插入点
			{
				y = x;
				flag = _comp(v.first,x->_pair.first);
				x = flag ? x->_left : x->_right;
			}
			iterator j = iterator(y);   //循环结束后y是插入点的父节点，必然是叶子节点
			if (flag)                   //表示遇到大的节点，插入点应该在y的左边
			{
				if (j == begin())
				{
					return _insert_(x, y, v);
				}
				else
				{
					--j;
				}
			}
			//插入点在y右边
			if (_comp(j->first, v.first))
			{
				return _insert_(x, y, v);
			}
			//进行到这里说明有key重复的节点,就析构value值，重新构造
			_allo.destroy(&j->second);
			_allo.construct(&j->second,v.second);
			return j;
		}

		//查找键值为key的节点
		inline iterator find(const Key& key) const noexcept
		{
			_pnode_ y = _header_,x = _header_->_parent;
			while(x)
			{
				if(!_comp(x->_pair.first,key))
				{
					y = x,x = x->_left;
				}
				else
				{
					x = x->_right;
				}
			}
			iterator j = iterator(y);
			return (j == end() || _comp(key,j._node->_pair.first)) ? end() : j;
		}

		//[]操作符，返回根据key返回对应的value
		inline Value& operator[](const Key& key) noexcept
		{
			iterator j = find(key);
			if(j == end())
			{
				j = insert(key,Value());
			}
			return j->second;
		}

		//[]操作符，返回根据key返回对应的value
		inline Value& operator[](const Key& key) const noexcept
		{
			iterator j = find(key);
			assert(j != end());
			return j->second;
		}

		//删除一个节点
		inline void erase(iterator position) noexcept
		{
			_pnode_ y = _rb_tree_rebalance_for_erase(position._node,_header_->_parent,_header_->_left,_header_->_right);
			_destroy_node_(y);
			--_n;
		}

		//判断目前的树形结构是不是红黑树
		inline bool is_rb_tree() const noexcept
		{
			return _is_rb_tree();
		}

	protected:

		using _pnode_ = _rb_tree_node<Key, Value>*;

		//真正执行clear的地方
		inline void _clear(_pnode_ x) noexcept
		{
			if(x == nullptr)
			{
				return;
			}
			_clear(x->_left);
			_clear(x->_right);
			_destroy_node_(x);
		}

		//申请一个红黑树节点
		inline _pnode_ __allocate_node_() noexcept
		{
			_pnode_ pt = static_cast<_pnode_>(_allo.allocate(sizeof(_rb_tree_node<Key, Value>)));
			return pt;
		}

		//构造一个红黑树节点
		inline _pnode_ _create_node_(const Key& k, const Value& v) noexcept
		{
			_pnode_ pt = __allocate_node_();
			_allo.construct(&pt->_pair.second, v);
			_allo.construct(&pt->_pair.first, k);
			return pt;
		}

		//摧毁一个红黑树节点
		inline void _destroy_node_(_pnode_ x) noexcept
		{
			_allo.destroy(&x->_pair.second);
			_allo.destroy(&x->_pair.first);
			_allo.deallocate(x);
		}

		//x_为新值插入点,y_为插入点父节点,v为值
		//真正执行插入的地方
		iterator _insert_(_pnode_ x_, _pnode_ y_, const pair<Key, Value>& v) noexcept
		{
			_pnode_ x = x_, y = y_, z = nullptr;
			if (y == _header_ || x != nullptr || _comp(v.first, y->_pair.first))
			{
				z = _create_node_(v.first, v.second);
				y->_left = z;
				if (y == _header_)
				{
					_header_->_parent = z;
					_header_->_right = z;
				}
				else if (y == _header_->_left)
				{
					_header_->_left = z;
				}
			}
			else
			{
				z = _create_node_(v.first, v.second);
				y->_right = z;
				if (y == _header_->_right)
				{
					_header_->_right = z;
				}
			}
			z->_parent = y;
			z->_left = nullptr;
			z->_right = nullptr;
			_rb_tree_rebalance(z, _header_->_parent);
			++_n;
			return iterator(z);
		}

		//红黑树插入后的调整
		void _rb_tree_rebalance(_pnode_ x, _pnode_& root) noexcept
		{
			x->_color = red;  //新节点一定为红
			while (x != root && x->_parent->_color == red)
			{
				if (x->_parent == x->_parent->_parent->_left)
				{
					_pnode_ y = x->_parent->_parent->_right;
					if (y && y->_color == red)
					{
						x->_parent->_color = black;
						y->_color = black;
						x->_parent->_parent->_color = red;
						x = x->_parent->_parent;
					}
					else
					{
						if (x == x->_parent->_right)
						{
							x = x->_parent;
							_rb_tree_rotate__left(x, root);
						}
						x->_parent->_color = black;
						x->_parent->_parent->_color = red;
						_rb_tree_rotate__right(x->_parent->_parent, root);
					}
				}
				else
				{
					_pnode_ y = x->_parent->_parent->_left;
					if (y && y->_color == red)
					{
						x->_parent->_color = black;
						y->_color = black;
						x->_parent->_parent->_color = red;
						x = x->_parent->_parent;
					}
					else
					{
						if (x == x->_parent->_left)
						{
							x = x->_parent;
							_rb_tree_rotate__right(x, root);
						}
						x->_parent->_color = black;
						x->_parent->_parent->_color = red;
						_rb_tree_rotate__left(x->_parent->_parent, root);
					}
				}
			}
			root->_color = black;
		}

		//左旋转
		void _rb_tree_rotate__left(_pnode_ x, _pnode_& root) noexcept
		{
			_pnode_ y = x->_right;
			x->_right = y->_left;
			if(y->_left)
			{
				y->_left->_parent = x;
			}
			y->_parent = x->_parent;
			if (x == root)
			{
				root = y;
			}
			else if (x == x->_parent->_left)
			{
				x->_parent->_left = y;
			}
			else
			{
				x->_parent->_right = y;
			}
			y->_left = x;
			x->_parent = y;
		}

		//右旋转
		void _rb_tree_rotate__right(_pnode_ x, _pnode_& root) noexcept
		{
			_pnode_ y = x->_left;
			x->_left = y->_right;
			if (y->_right)
			{
				y->_right->_parent = x;
			}
			y->_parent = x->_parent;
			if (x == root)
			{
				root = y;
			}
			else if (x == x->_parent->_right)
			{
				x->_parent->_right = y;
			}
			else
			{
				x->_parent->_left = y;
			}
			y->_right = x;
			x->_parent = y;
		}

		//红黑树删除一个节点过后的调整
		_pnode_ _rb_tree_rebalance_for_erase(_pnode_ z,_pnode_& root,_pnode_& _leftmost,_pnode_& _rightmost) noexcept
		{
			_pnode_ y = z,x = nullptr,x__parent = nullptr;
			if(y->_left == nullptr)
			{
				x = y->_right;
			}
			else
			{
				if(y->_right == nullptr)
				{
					x = y->_left;
				}
				else
				{
					y = y->_right;
					while(y->_left)
					{
						y = y->_left;
					}
					x = y->_right;
				}
			}
			if(y != z)
			{
				z->_left->_parent = y;
				y->_left = z->_left;
				if(y != z->_right)
				{
					x__parent = y->_parent;
					if(x)
					{
						x->_parent = y->_parent;
					}
					y->_parent->_left = x;
					y->_right = z->_right;
					z->_right->_parent = y;
				}
				else
				{
					x__parent = y;
				}
				if(root = z)
				{
					root = y;
				}
				else if(z->_parent->_left == z)
				{
					z->_parent->_left = y;
				}
				else
				{
					z->_parent->_right = y;
				}
				y->_parent = z->_parent;
				swap(y->_color,z->_color);
				y = z;
			}
			else
			{
				x__parent = y->_parent;
				if(x)
				{
					x->_parent = y->_parent;
				}
				if(root == z)
				{
					root = x;
				}
				else
				{
					if(z->_parent->_left == z)
					{
						z->_parent->_left = x;
					}
					else
					{
						z->_parent->_right = x;
					}
				}
				if(_leftmost == z)
				{
					if(z->_right == nullptr)
					{
						_leftmost = z->_parent;
					}
					else
					{
						_leftmost = _minimum(x);
					}
				}
				if(_rightmost == z)
				{
					if(z->_left == nullptr)
					{
						_rightmost = z->_parent;
					}
					else
					{
						_rightmost = _maximum(x);
					}
				}
			}
			if (y->_color != red)
			{
				while (x != root && (x == 0 || x->_color == black))
				{
					if (x == x__parent->_left)
					{
						_pnode_ w = x__parent->_right;
						if (w->_color == red)
						{
							w->_color = black;
							x__parent->_color = red;
							_rb_tree_rotate__left(x__parent, root);
							w = x__parent->_right;
						}
						if ((w->_left == 0 || w->_left->_color == black) &&
								(w->_right == 0 || w->_right->_color == black))
						{
							w->_color = red;
							x = x__parent;
							x__parent = x__parent->_parent;
						}
						else
						{
							if (w->_right == 0 || w->_right->_color == black)
							{
								if (w->_left)
								{
									w->_left->_color = black;
								}
								w->_color = red;
								_rb_tree_rotate__right(w, root);
								w = x__parent->_right;
							}
							w->_color = x__parent->_color;
							x__parent->_color = black;
							if (w->_right)
							{
								w->_right->_color = black;
							}
							_rb_tree_rotate__left(x__parent, root);
							break;
						}
					}
					else
					{               
						_pnode_ w = x__parent->_left;
						if (w->_color == red)
						{
							w->_color = black;
							x__parent->_color = red;
							_rb_tree_rotate__right(x__parent, root);
							w = x__parent->_left;
						}
						if ((w->_right == 0 || w->_right->_color == black) &&
								(w->_left == 0 || w->_left->_color == black))
						{
							w->_color = red;
							x = x__parent;
							x__parent = x__parent->_parent;
						}
						else
						{
							if (w->_left == 0 || w->_left->_color == black)
							{
								if (w->_right)
								{
									w->_right->_color = black;
								}
								w->_color = red;
								_rb_tree_rotate__left(w, root);
								w = x__parent->_left;
							}
							w->_color = x__parent->_color;
							x__parent->_color = black;
							if (w->_left)
							{
								w->_left->_color = black;
							}
							_rb_tree_rotate__right(x__parent, root);
							break;
						}
					}
				}
				if (x)
				{
					x->_color = black;
				}
			}
			return y;
		}

		//返回x的最左边子节点
		static inline _pnode_ _minimum(_pnode_ x) noexcept
		{
			if(x)
			{
				while(x->_left != nullptr)
				{
					x = x->_left;
				}
			}
			return x;
		}

		//返回x的最右边子节点
		static inline _pnode_ _maximum(_pnode_ x) noexcept
		{
			if(x)
			{
				while(x->_right != nullptr)
				{
					x = x->_right;
				}
			}
			return x;
		}

		//真正执行判断一个树是不是红黑树的地方
		bool _is_rb_tree() const noexcept
		{
			if(_n == 0 || _header_->_left == _header_)
			{
				return _n == 0 && _header_->_left == _header_ && _header_->_right == _header_;
			}
			size_t len = _black_count(_header_->_left,_header_->_parent);
			_pnode_ x = nullptr,L = nullptr,R = nullptr;
			for(iterator i = begin();i != end();++i)
			{
				x = i._node;
				L = x->_left;
				R = x->_right;
				if(x->_color == red)
				{
					if((L && L->_color == red) || (R && R->_color == red))
					{
						return false;
					}
				}
				if(L && _comp(x->_pair.first, L->_pair.first))
				{
					return false;
				}
				if(R && !_comp(x->_pair.first,R->_pair.first))
				{
					return false;
				}
				if(!L && !R && _black_count(x,_header_->_parent) != len)
				{
					return false;
				}
				if(_header_->_left != _minimum(_header_->_parent))
				{
					return false;
				}
				if(_header_->_right != _maximum(_header_->_parent))
				{
					return false;
				}
			}
			return true;
		}

		//统计黑色节点数量
		size_t _black_count(_pnode_ node,_pnode_ root) const noexcept
		{
			if(node == nullptr)
			{
				return 0;
			}
			else
			{
				size_t bc = node->_color == black ? 1 : 0;
				if(node == root)
				{
					return bc;
				}
				else
				{
					return bc + _black_count(node->_parent,root);
				}
			}
		}

	private:
		_pnode_ _header_;   //头节点，是一个实现技巧
		size_t _n;	   //红黑树节点数量
		Alloc _allo;  //空间配置器
		Comp _comp;  //键值比较规则
};
#endif



