#ifndef _VECTOR_H_
#define _VECTOR_H_
#include"memory.h"
#include"iterator_traits.h"
#include"type_traits.h"
#include"algorithm.h"
#include"functional.h"
#include"allocator.h"
#include<cassert>
namespace my_stl
{

	//向量容器，动态数组
	template<class T,class Alloc = allocator<T>>
		class vector
		{
			public:

				using value_type = T;
				using pointer = T*;
				using reference = T&;
				using iterator = T*;
				using distance_type = ptrdiff_t;
				using size_type = size_t;
				using const_pointer = const T*;
				using const_reference = const T&;

				static_assert(!is_const_v<T>);
				static_assert(is_same_v<T,typename Alloc::value_type>);
				static_assert(!is_void_v<T>);
				static_assert(!is_same_v<void*,T>);


				//默认构造函数
				inline vector() noexcept : _first(nullptr),_last(nullptr),_end(nullptr) {}

				//移动构造函数
				inline vector(vector&& rhs) noexcept : _first(rhs._first),_last(rhs._last),_end(rhs._end)
				{
					rhs._first = rhs._last = rhs._end = nullptr;
				}

				//operator = 
				inline vector& operator=(const vector& rhs) noexcept
				{
					if(this != &rhs)
					{
						clear();
						if(rhs.size() == 0 || !rhs._first)
						{
							return *this;
						}
						_first = static_cast<T*>(allo.allocate(rhs.size() * sizeof(T)));
						uninitialized_copy(rhs._first,rhs._last,_first);
						_last = _end = _first + rhs.size();
					}
					return *this;
				}

				//移动赋值
				inline vector& operator=(vector&& rhs) noexcept
				{
					if(this != &rhs)
					{
						clear();
						_first = rhs._first;
						_last = rhs._last;
						_end = rhs._end;
						rhs._last = rhs._first = rhs._end = nullptr;
					}
					return *this;
				}

				//构造函数，构造能够容纳n个元素的vector
				template<class Size>
					inline vector(Size n) noexcept
					{
						if(n <= 0)
						{
							_first = _last = _end = nullptr;
							return;
						}
						_first = _last = static_cast<T*>(allo.allocate(n * 2 * sizeof(T)));
						_end = _first + n * 2;
					}

				//使用范围[first,last)内的元素来初始化vector
				inline vector(iterator first,iterator last) noexcept
				{
					ptrdiff_t n = last - first;
					if(n <= 0)
					{
						_first = _last = _end = nullptr;
						return;
					}
					_first = static_cast<T*>(allo.allocate(n * 2 * sizeof(T)));
					my_stl::uninitialized_copy(first,last,_first);
					_last = _first + n;
					_end = _first + 2 * n;
				}

				//拷贝构造函数，使用另一个vector来初始化vector
				inline vector(const vector& rhs) noexcept
				{
					ptrdiff_t n = rhs.size();
					if(n <= 0)
					{
						_first = _last = _end = nullptr;
						return;
					}
					_first = static_cast<T*>(allo.allocate(n * 2 * sizeof(T)));
					my_stl::uninitialized_copy(rhs._first,rhs._last,_first);
					_last = _first + n;
					_end = _first + 2 * n;
				}

				//构造出n个元素值为val的vector
				template<class Size>
					inline vector(Size n,const T& val) noexcept
					{
						if(n <= 0)
						{
							_first = _last = _end = nullptr;
							return;
						}
						_first = static_cast<T*>(allo.allocate(n * 2 * sizeof(T)));
						my_stl::uninitialized_fill_n(_first,n,val);
						_last = _first + n;
						_end = _first + 2 * n;
					}

				//析构函数
				~vector() noexcept
				{
					clear();
				}

				//清空vector
				inline void clear() noexcept
				{
					if(_first)
					{
						allo.destroy(_first,_last);
						allo.deallocate(_first);
					}
					_first = _last = _end = nullptr;
				}

				//返回首迭代器
				inline iterator begin() const
				{
					return _first;
				}

				//返回尾迭代器
				inline iterator end() const
				{
					return _last;
				}

				//返回反向首迭代器
				inline iterator rbegin() const
				{
					return _last - 1;
				}

				//返回反向尾迭代器
				inline iterator rend() const
				{
					_first - 1;
				}

				//返回vector的size
				inline size_type size() const
				{
					return static_cast<size_type>(_last - _first);
				}

				//返回vector的容量
				inline size_type capacity() const
				{
					return static_cast<size_type>(_end - _first);
				}

				//返回vector是否为空
				inline bool empty() const
				{
					return _first == _last;
				}

				//重新指定vector的size，超过的使用默认值来填充
				inline void resize(size_t n) noexcept
				{
					resize(n,T());
				}

				//真正执行resize的地方
				void resize(size_t n,const T& val) noexcept
				{
					if(n > size())
					{
						for(size_t i = size();i < n;++i)
						{
							push_back(val);
						}
					}
					else if(n < size())
					{
						erase(begin() + n,end());
					}
				}

				//sort,使用快速排序,可以指定排序规则
				template<class Comp>
					inline void sort(const Comp& comp) noexcept
					{
						if(!_first || size() < 2)
						{
							return;
						}
						my_stl::sort(_first,_last,comp);
					}

				//sort,使用快速排序,默认规则为less
				inline void sort() noexcept
				{
					if(!_first || size() < 2)
					{
						return;
					}
					my_stl::sort(_first,_last,less<T>());
				}

				//移除迭代器所指的元素
				inline void erase(iterator pos) noexcept
				{
					if(!_first || _first == _last)
					{
						return;
					}
					_erase_(pos);
				}

				//移除元素值为val的第一个元素
				inline void erase(const T& val) noexcept
				{
					if(!_first || _first == _last)
					{
						return;
					}
					iterator pt = my_stl::find(_first,_last,val);
					if(pt != _last)
					{
						_erase_(pt);
					}
				}

				//移除迭代器范围[first,last)内的元素
				void erase(iterator first,iterator last) noexcept
				{
					if(!_first || !first || !last || distance(first,last) <= 0)
					{
						return;
					}
					iterator i = copy(last,_last,first);
					allo.destroy(i,_last);
					_last = _last - (last - first);
				}

				//删除vector最后一个元素
				inline void pop_back() noexcept
				{
					if(!_first || _first == _last)
					{
						return;
					}
					_erase_(_last - 1);
				}

				//在尾部插入一个元素
				void push_back(const T& val) noexcept
				{
					if(_first == nullptr)
					{
						_first = _last = static_cast<T*>(allo.allocate(10 * sizeof(T)));
						_end = _first + 10;
					}
					if(_last + 1 <= _end)
					{
						allo.construct(_last,val);
						++_last;
					}
					else
					{
						size_t n = _end - _first;
						iterator old_first = _first,old_last = _last;
						_first = static_cast<T*>(allo.allocate(n * 2 * sizeof(T)));
						uninitialized_copy(old_first,old_last,_first);
						_last = _first + (old_last - old_first);
						allo.construct(_last++,val);
						_end = _first + 2 * n;
						allo.destroy(old_first,old_last);
						allo.deallocate(old_first);
					}
				}

				//重载operator []操作符
				inline reference operator[](size_t index) noexcept
				{
					assert(_first && index <= (size() -1) && index >= 0);
					return *(_first + index);
				}

				//重载operator []操作符
				inline const_reference operator[](size_t index) const noexcept
				{
					assert(_first && index <= (size() - 1) && index >= 0);
					return *(_first + index);
				}

				//返回vector第一个元素
				inline reference front()const noexcept
				{
					assert(_first && size() != 0);
					return *_first;
				}

				//返回vector最后一个元素
				inline reference back()const noexcept
				{
					assert(_first && size() != 0);
					return *(_last - 1);
				}

				//查找元素val，返回迭代器
				inline iterator find(const T& val)const  noexcept
				{
					if(!_first || _first == _last)
					{
						return _last;
					}
					return my_stl::find(_first,_last,val);
				}

				//使size等于capacity
				inline void shrink_to_fit() noexcept
				{
					if(!_first || _first == _last)
					{
						return;
					}
					iterator f = _first,l = _last;
					_first = static_cast<T*>(allo.allocate((l - f) * sizeof(T)));
					uninitialized_copy(f,l,_first);
					_end = _last = _first + (l - f);
					allo.destroy(f,l);
					allo.deallocate(f);
				}

				//交换两个vector
				inline void swap(vector& rhs)noexcept
				{
					auto f = _first,l = _last,e = _end;
					_first = rhs._first;
					_last = rhs._last;
					_end = rhs._end;
					rhs._end = e;
					rhs._first = f;
					rhs._last = l;
				}

			protected:

				//真正的erase操作
				inline void _erase_(iterator pt) noexcept
				{
					copy(pt + 1,_last,pt);
					allo.destroy(--_last);
				}

			private:
				//维护三个指针和一个空间适配器
				Alloc allo;
				T* _first;
				T* _last;
				T* _end;
		};

};
#endif
