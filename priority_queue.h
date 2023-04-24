#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_
#include"vector.h"
namespace my_stl
{

	//优先级队列，底层使用vector容器+heap算法+less比较实现
	template<class T,class Comp = less<T>,class Container = vector<T>>
		class priority_queue
		{
		public:

			using value_type = typename Container::value_type;
			using iterator = typename Container::iterator;
			using distance_type = typename Container::distance_type;
			using pointer = typename Container::pointer;
			using reference = typename Container::reference;
			using size_type = typename Container::size_type;
			
			static_assert(is_same_v<T,value_type>);
			static_assert(!is_void_v<T>);

			inline explicit priority_queue(): _container(),_comp() {}

			inline priority_queue(iterator first,iterator last,const Comp& comp) noexcept : _container(first,last),_comp(comp)
			{
				make_heap(_container.begin(),_container.end(),_comp);
			}

			inline priority_queue(iterator first,iterator last) noexcept : _container(first,last),_comp()
			{
				make_heap(_container.begin(),_container.end(),_comp);
			}

			inline priority_queue(const priority_queue& rhs) : _container(rhs._container),_comp(rhs._comp) {}

			inline ~priority_queue() noexcept {}

			inline priority_queue& operator=(const priority_queue& rhs) noexcept
			{
				if(this != &rhs)
				{
					_container = rhs._container;
					_comp = rhs._comp;
				}
				return *this;
			}

			//添加一个元素，并维护堆的性质
			inline void push(const T& val)
			{
				_container.push_back(val);
				push_heap(_container.begin(),_container.end(),_comp);
			}

			//弹出优先级最大的元素
			inline T& top() const
			{
				return _container.front();
			}

			//删除优先级最大的函数，并且维护堆的性质
			inline void pop() noexcept
			{
				pop_heap(_container.begin(),_container.end(),_comp);
				_container.pop_back();
			}

			inline size_t size() const 
			{
				return _container.size();
			}

			inline bool empty() const
			{
				return _container.empty(); 
			}

			inline void clear() noexcept
			{
				_container.clear();
			}

		protected:
			//测试函数
			iterator begin() const
			{
				return _container.begin();
			}

			//测试函数
			iterator end() const
			{
				return _container.end();
			}

		private:
			Comp _comp;
			Container _container;
		};

};
#endif
