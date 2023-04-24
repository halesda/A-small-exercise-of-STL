#ifndef _QUEUE_H_
#define _QUEUE_H_
#include"list.h"
namespace my_stl
{
	//queue
	template<class T,class Container = list<T>>
		class queue
		{
		public:

			using value_type = typename Container::value_type;
			using reference = typename Container::reference;
			using size_type = typename Container::size_type;
			using pointer = typename Container::pointer;
			using distance_type = typename Container::distance_type;
			static_assert(is_same_v<T,typename Container::value_type>);

			inline queue() noexcept : _con() {}

			inline queue(const queue& rhs) noexcept : _con(rhs._con) {}

			inline ~queue() noexcept
			{
				_con.clear();
			}

			inline queue& operator =(const queue& rhs) noexcept
			{
				if(this != &rhs)
				{
					_con = rhs._con;
				}
				return *this;
			}

			inline void push(const T& val) noexcept
			{
				_con.push_back(val);
			}

			inline value_type& front() const noexcept
			{
				return _con.front();
			}

			inline void pop() noexcept
			{
				_con.pop_front();
			}

			inline size_type size() const noexcept
			{
				return _con.size();
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
