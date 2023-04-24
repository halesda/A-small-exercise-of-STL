#ifndef _MEMORY_H_
#define _MEMORY_H_
#include"type_traits.h"
#include"iterator_traits.h"
#include<new>
namespace my_stl
{

	template<class T1,class T2>
		inline void construct(T1* pt,const T2& value) noexcept
		{
			new (pt)T1(value);
		}

	//construct
	template<class ForwardIterator,class T>
		inline void construct(ForwardIterator first,ForwardIterator last,const T& val) noexcept
		{
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_pod_v<_valtype>)
			{
				for(;first != last;++first)
				{
					*first = val;
				}
			}
			else
			{
				for(;first != last;++first)
				{
					construct(&(*first),val);
				}
			}
		}

	template<class T>
		inline void destroy(T* pt) noexcept
		{
			pt->~T();
		}

	//destroy
	template<class ForwardIterator>
		inline void destroy(ForwardIterator first,ForwardIterator last) noexcept
		{
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_pod_v<_valtype>) {}
			else
			{
				for(;first != last;++first)
				{
					destroy(&(*first));
				}
			}
		}

	//uninitialized_fill
	template<class ForwardIterator,class T>
		inline ForwardIterator uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& val) noexcept
		{
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_pod_v<_valtype>)
			{
				for(;first != last;++first)
				{
					*first = val;
				}
				return first;
			}
			else
			{
				for(;first != last;++first)
				{
					construct(&(*first),val);
				}
				return first;
			}
		}

	//uninitialized_fill_n
	template<class ForwardIterator,class Size,class T>
		inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& val) noexcept
		{
			if(n <= 0)
			{
				return first;
			}
			using _itertype = typename  iterator_traits<ForwardIterator>::iterator_category;
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_random_access_iterator_v<_itertype>)
			{
				return uninitialized_fill(first,first + n,val);
			}
			else
			{
				if constexpr(is_pod_v<_valtype>)
				{
					for(;n > 0;--n,++first)
					{
						*first = val;
					}
					return first;
				}
				else
				{
					for(;n > 0;--n,++first)
					{
						construct(&(*first),val);
					}
					return first;
				}
			}
		}

	//uninitialized_copy_n
	template<class ForwardIterator,class Size,class InputIterator>
		inline ForwardIterator uninitialized_copy_n(InputIterator first,Size size,ForwardIterator dest) noexcept
		{
			if(size <= 0)
			{
				return dest;
			}
			using _valtype = typename iterator_traits<InputIterator>::value_type;
			if constexpr (is_pod_v<_valtype>)
			{
				for(;size > 0;--size)
				{
					*dest = *first;
					++first;++dest;
				}
				return dest;
			}
			else
			{
				for(;size > 0;--size)
				{
					construct(&(*dest),*first);
					++dest;++first;
				}
				return dest;
			}
		}

	//uninitialized_copy
	template<class ForwardIterator,class InputIterator>
		inline ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator dest) noexcept
		{
			using _valtype = typename iterator_traits<InputIterator>::value_type;
			using _itertype = typename iterator_traits<InputIterator>::iterator_category;

			if constexpr(is_random_access_iterator_v<_itertype>)
			{
				return uninitialized_copy_n(first,(last - first),dest);
			}
			else
			{
				if constexpr(is_pod_v<_valtype>)
				{
					for(;first != last;++first,++dest)
					{
						*dest = *first;
					}
					return dest;
				}
				else
				{
					for(;first != last;++first,++dest)
					{
						construct(&(*dest),*first);
					}
					return dest;
				}
			}
		}

	//copy
	template<class ForwardIterator,class InputIterator>
		inline ForwardIterator copy(InputIterator first,InputIterator last,ForwardIterator dest) noexcept
		{
			for(;first != last;++first,++dest)
			{
				*dest = *first;
			}
			return dest;
		}

	//fill
	template<class ForwardIterator,class T>
		inline ForwardIterator fill(ForwardIterator first,ForwardIterator last,const T& val) noexcept
		{
			for(;first != last;++first)
			{
				*first = val;
			}
			return first;
		}

	//move
	template <typename T>
		inline typename remove_reference<T>::type&& move(T&& arg) noexcept
		{
			return static_cast<typename remove_reference<T>::type&&>(arg);
		}

	//forward
	//C++11 引入了完美转发（Perfect Forwarding）的概念，可以让我们在函数模板中实现类型转发，将参数按照原来的类型转发给其他函数，从而避免了参数类型丢失和额外的拷贝。forward 是实现完美转发的重要工具之一。forward 是一个函数模板，用于将传入的参数转发为对应的左值或右值引用，以实现完美转发。
	template <typename T>
		inline T&& forward(typename remove_reference<T>::type& arg) noexcept
		{
			return static_cast<T&&>(arg);
		}

	template <typename T>
		inline T&& forward(typename remove_reference<T>::type&& arg) noexcept
		{
			return static_cast<T&&>(arg);
		}


};
#endif
