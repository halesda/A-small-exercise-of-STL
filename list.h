#ifndef _LIST_H_
#define _LIST_H_
#include"type_traits.h"
#include"memory.h"
#include"allocator.h"
#include"functional.h"
#include"iterator_traits.h"
#include<cassert>
#include"algorithm.h"
namespace my_stl
{

	//数据结构双向循环链表
	
	//节点设计
	template<class T>
	struct _list_node_
	{
		using value_type = T;
		_list_node_<T>* _next;
		_list_node_<T>* _prev;
		T data;
	};

	//迭代器设计
	template<class T>
	class _list_iterator_
	{
	public:

		using value_type = T;
		using pointer = T*;
		using distance_type = ptrdiff_t;
		using iterator_category = bidirectional_iterator_tag;
		using reference = T&;
		using _node_ptr_ = _list_node_<T>*;

		inline _list_iterator_(const _node_ptr_& p = nullptr) noexcept :_pt(p){}

		inline ~_list_iterator_() noexcept {}

		inline _list_iterator_(const _list_iterator_& rhs) noexcept: _pt(rhs._pt) {}

		inline _list_iterator_(_list_iterator_&& rhs) noexcept : _pt(rhs._pt)
		{
			rhs._pt = nullptr;
		}

		inline _list_iterator_& operator =(const _list_iterator_& rhs) noexcept
		{
			if(this != &rhs)
			{
				_pt = rhs._pt;
			}
			return *this;
		}

		inline _list_iterator_& operator=(_list_iterator_&& rhs) noexcept
		{
			if(this != &rhs)
			{
				_pt = rhs._pt;
				rhs._pt = nullptr;
			}
			return *this;
		}

		inline _list_iterator_& operator ++() noexcept
		{
			if(_pt)
			{
				_pt = _pt->_next;
			}
			return *this;
		}

		inline const _list_iterator_ operator ++(int) noexcept
		{
			_list_iterator_ tmp(*this);
			++(*this);
			return tmp;
		}

		inline _list_iterator_& operator-- ()noexcept
		{
			if(_pt)
			{
				_pt = _pt->_prev;
			}
			return *this;
		}

		inline const _list_iterator_ operator--(int) noexcept
		{
			_list_iterator_ tmp(*this);
			--(*this);
			return tmp;
		}

		inline value_type& operator*() noexcept
		{
			assert(_pt);
			return _pt->data;
		}

		inline const value_type& operator*() const noexcept
		{
			assert(_pt);
			return _pt->data;
		}

		inline bool operator ==(const _list_iterator_& rhs)const  noexcept
		{
			return _pt == rhs._pt;
		}

		inline bool operator !=(const _list_iterator_& rhs) const noexcept
		{
			return _pt != rhs._pt;
		}

		inline _node_ptr_ _get_ptr_()const noexcept
		{
			return _pt;
		}

	private:
		_node_ptr_ _pt;
	};

	//双向循环链表
	template<class T,class Alloc = allocator<T>>
	class list
	{
	public:

		using reference = T&;
		using _node_ptr_ = _list_node_<T>*;
		using size_type = size_t;
		using iterator = _list_iterator_<T>;
		using distance_type = ptrdiff_t;
		using value_type = T;
		using pointer = T*;

		static_assert(is_same_v<T,typename Alloc::value_type>);
		static_assert(is_same_v<T,typename iterator::value_type>);
		static_assert(!is_same_v<T,void>);
		static_assert(!is_const_v<T>);
		static_assert(is_same_v<T,typename _list_node_<T>::value_type>);

		//默认构造
		inline list() noexcept :_head(static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)))),sizes(0)
		{
			_head->_next = _head;
			_head->_prev = _head;
		}

		//构造n个节点值为val
		template<class Size>
		inline list(Size n,const T& val)noexcept : _head(nullptr),sizes(0)
		{
			for(;n > 0;--n)
			{
				push_back(val);
			}
		}

		//拷贝构造
		inline list(const list& rhs) noexcept : _head(nullptr),sizes(0)
		{
			for(auto i = rhs.begin();i != rhs.end();++i)
			{
				push_back(*i);
			}
		}

		//拷贝赋值
		inline list& operator =(const list& rhs) noexcept
		{
			if(this != &rhs)
			{
				clear();
				for(auto i = rhs.begin();i != rhs.end();++i)
				{
					push_back(*i);
				}
			}
			return *this;
		}

		//析构函数
		inline ~list() noexcept
		{
			clear();
			if(_head)
			{
				allo.deallocate(_head);
			}
			_head = nullptr;
			sizes = 0;
		}

		//清空整个链表函数
		void clear() noexcept
		{
			if(_head == nullptr)
			{
				return;
			}
			if(_head && sizes == 0)
			{
				_head->_next = _head;
				_head->_prev = _head;
				return;
			}
			if(sizes == 0)
			{
				return;
			}
			//开始清理
			_node_ptr_ next = nullptr;
			for(_node_ptr_ i =_head->_next;i != _head;)
			{
				next = i->_next;
				if(i)
				{
					allo.destroy(&i->data);
					allo.deallocate(i);
				}
				i = next;
			}
			_head->_next = _head;
			_head->_prev = _head;
			sizes = 0;
			return;
		}

		//返回首迭代器
		inline iterator begin() const
		{
			assert(_head);
			return iterator(_head->_next);
		}

		//返回尾迭代器
		inline iterator end() const
		{
			assert(_head);
			return iterator(_head);
		}

		//返回反向首迭代器
		inline iterator rbegin() const
		{
			assert(_head);
			return iterator(_head->_prev);
		}

		//返回反向尾迭代器
		inline iterator rend() const
		{
			assert(_head);
			return iterator(_head);
		}

		//在链表尾部插入一个元素
		void push_back(const T& val) noexcept
		{
			if(_head == nullptr)
			{
				_head = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				_head->_next = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				_head->_prev = _head->_next;
				_head->_next->_next = _head;
				_head->_next->_prev = _head;
				allo.construct(&_head->_next->data,val);
				sizes = 1;
			}
			else
			{
				_node_ptr_ i = _head->_prev;
				i->_next = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				i->_next->_prev = i;
				i->_next->_next = _head;
				_head->_prev = i->_next;
				allo.construct(&i->_next->data,val);
				++sizes;
			}
		}

		//在链表头部插入一个元素
		void push_front(const T& val) noexcept
		{
			if(_head == nullptr)
			{
				_head = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				_head->_next = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				_head->_prev = _head->_next;
				_head->_next->_next = _head;
				_head->_next->_prev = _head;
				allo.construct(&_head->_next->data,val);
				sizes = 1;
			}
			else
			{
				_node_ptr_ first = _head->_next;
				_head->_next = static_cast<_node_ptr_>(allo.allocate(sizeof(_list_node_<T>)));
				_head->_next->_next = first;
				first->_prev = _head->_next;
				_head->_next->_prev = _head;
				++sizes;
				allo.construct(&_head->_next->data,val);
			}
		}

		//删除链表尾部的一个元素
		inline void pop_back() noexcept
		{
			erase(iterator(_head->_prev));
		}

		//删除链表头部的一个元素
		inline void pop_front() noexcept
		{
			erase(iterator(_head->_next));
		}

		//删除迭代器iter指向的元素
		inline void erase(iterator iter) noexcept
		{
			auto i = iter._get_ptr_();
			if(sizes == 0 || _head == nullptr || i == _head || i == nullptr)
			{
				return;
			}
			i->_next->_prev = i->_prev;
			i->_prev->_next = i->_next;
			allo.destroy(&i->data);
			allo.deallocate(i);
			--sizes;
		}

		//删除值为val的第一个元素
		inline void remove(const T& val) noexcept
		{
			auto iter = find(val);
			erase(iter);
		}

		//返回链表节点数量
		inline size_t size() const
		{
			return sizes;
		}

		//链表查找元素为val的节点，返回迭代器
		inline iterator find(const T& val)const noexcept
		{
			if(sizes == 0 || _head == nullptr)
			{
				return end();
			}
			return my_stl::find(begin(),end(),val);
		}

		//链表排序，默认使用less
		inline void sort() noexcept
		{
			if(_head == nullptr || sizes < 2)
			{
				return;
			}
			_quick_sort_(_head->_next,_head,less<value_type>());
		}

		//链表排序，可以指定排序规则
		template<class Comp>
		inline void sort(const Comp& comp) noexcept
		{
			if(_head == nullptr || sizes < 2)
			{
				return;
			}
			_quick_sort_(_head->_next,_head,comp);
		}

		//返回链表第一个元素
		reference front() const 
		{
			assert(sizes > 0);
			return _head->_next->data;
		}

		//返回链表最后一个元素
		reference back() const
		{
			assert(sizes > 0);
			return _head->_prev->data;
		}

		//链表是否为空
		bool empty() const noexcept
		{
			return sizes == 0;
		}
 
	protected:

		//链表排序，使用快速排序
		template<class Comp>
		void _quick_sort_(_node_ptr_ first,_node_ptr_ last,const Comp& comp) noexcept
		{
			if(first == last || first->_next == last || first == _head || last->_prev == _head || (last->_next == first && first != _head->_next))
			{
				return;
			}
			auto tmp = first->data;
			auto left = first,right = last->_prev;
			bool flag = true;
			while(left != right)
			{
				if(flag)
				{
					if(comp(tmp,right->data))
					{
						right = right->_prev;
						continue;
					}
					left->data = right->data;
					left = left->_next;
					flag = false;
				}
				else
				{
					if(comp(left->data,tmp))
					{
						left = left->_next;
						continue;
					}
					right->data = left->data;
					right = right->_prev;
					flag = true;
				}
			}
			if(left != first)
			{
				left->data = tmp;
			}
			_quick_sort_(first,left,comp);
			_quick_sort_(left->_next,last,comp);
		}


	private:
		//维护三个成员
		//头节点指针和sizes大小和空间适配器
		_node_ptr_ _head;
		size_t sizes;
		Alloc allo;
	};

};
#endif
