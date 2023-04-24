#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
#include"./memory.h"   
#include<iostream>
using std::cout,std::endl;
namespace my_stl
{

	//简易的空间适配器,真的非常简单，没什么技术含量...
	template<class T>
	struct allocator
	{
		//规定出一些接口
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using size_type = size_t;
		using distance_type = ptrdiff_t;
		using const_pointer = const T*;
		using const_reference = const T&;
	
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

		template<class T1>
		inline static T1* address(const T1& val) noexcept
		{
			return static_cast<T1*>(&val);
		}

		//out of memory
		//申请内存失败的话打印出三条提示信息然后强制退出程序。。。
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
