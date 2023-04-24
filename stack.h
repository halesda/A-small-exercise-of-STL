#ifndef _STACK_H_
#define _STACK_H_
#include"list.h"
namespace my_stl
{

	//数据结构栈，底层容器是list
	//对部分功能做了限制
	template<class T,class Container = list<T>>
		class stack
		{
			public:

				static_assert(is_same_v<T,typename Container::value_type>);
				using value_type = typename Container::value_type;
				using size_type = typename Container::size_type;
				using reference = typename Container::reference;
				using pointer = typename Container::pointer;
				using distance_type = typename Container::distance_type;

				inline stack() {}

				inline stack(const stack& rhs) noexcept : _con(rhs._con) {}

				inline stack& operator =(const stack& rhs) noexcept
				{
					if(this != &rhs)
					{
						_con = rhs._con;
					}
					return *this;
				}

				inline ~stack() noexcept
				{
					_con.clear();
				}

				inline void push(const T& val) noexcept
				{
					_con.push_back(val);
				}

				inline size_type size() const noexcept
				{
					return _con.size();
				}

				inline void pop() noexcept
				{
					_con.pop_back();
				}

				inline T& top() noexcept
				{
					return _con.back();
				}

				inline T& top() const noexcept
				{
					return _con.back();
				}

				inline bool empty() const noexcept
				{
					return _con.empty();
				}

			private:

				Container _con;

		};
};
#endif
