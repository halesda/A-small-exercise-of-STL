#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
#include"./memory.h"   
#include<iostream>
using std::cout,std::endl;
namespace my_stl
{

	//简易的空间适配器
	template<class T>
	struct allocator
	{
		
		using value_type = T;
		static_assert(!is_const_v<T>);
		static_assert(!is_void_v<T>);

		//申请内存空间
		inline static void* allocate(size_t size) noexcept
		{
			void* pt = calloc(size,1);
			if(pt == nullptr)
			{
				out_of_memory();
			}
			return pt;
		}

		//释放内存空间
		template<class T1>
		inline static void deallocate(T1* pt) noexcept
		{
			free(pt);
		}

		//在空间上构造对象
		template<class T1,class T2>
		inline static void construct(T1* pt,const T2& val) noexcept
		{
			my_stl::construct(pt,val);
		}

		//在空间上构造对象
		template<class T1,class T2>
		inline static void construct(T1* first,T1* last,const T2& val) noexcept
		{
			my_stl::construct(first,last,val);
		}

		//在空间上析构对象
		template<class T1>
		inline static void destroy(T1* pt) noexcept
		{
			my_stl::destroy(pt);
		}

		//在空间上析构对象
		template<class T1,class T2>
		inline static void destroy(T1* first,T2* last) noexcept
		{
			my_stl::destroy(first,last);
		}

		//out of memory
		inline static void out_of_memory() noexcept
		{
			cout << "out of memory!" << endl;
			cout << "out of memory!" << endl;
			cout << "out of memory!" << endl;
			exit(0);
		}

	};

};
#endif
