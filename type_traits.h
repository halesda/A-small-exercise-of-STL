#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
namespace my_stl
{

	//判断是不是const类型
	template<class T>
		struct is_const
		{
			static constexpr bool value = false;
		};

	template<class T>
		struct is_const<const T>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_const<const T*>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_const<T* const>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_const<const T* const>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_const_v = is_const<T>::value;

	//判断两个类型是否相同
	template<class T1,class T2>
		struct is_same
		{
			static constexpr bool value = false;
		};

	template<class T>
		struct is_same<T,T>
		{
			static constexpr bool value = true;
		};

	template<class T1,class T2>
		static constexpr bool is_same_v = is_same<T1,T2>::value;

	//判断是不是整型类型
	template<class T>
		struct is_integral
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_integral<int>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const int>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<bool>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const bool>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<char>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const char>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<int64_t>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<unsigned int64_t>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const int64_t>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<short>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const short>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<unsigned short>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_integral<const unsigned short>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool  is_integral_v = is_integral<T>::value;


	//判断是不是指针类型
	template<class T>
		struct is_pointer
		{
			static constexpr bool value = false;
		};

	template<class T>
		struct is_pointer<T*>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_pointer<const T*>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_pointer<T* const>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_pointer<T* volatile>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_pointer<T* volatile const>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_pointer<const T* const>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_pointer_v = is_pointer<T>::value;

	//判断是不是void
	template<class T>
		struct is_void
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_void<void>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_void_v = is_void<T>::value;

	//判断是不是floating point
	template<class T>
		struct is_floating_point
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_floating_point<float>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_floating_point<double>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_floating_point<const float>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_floating_point<const double>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_floating_point_v = is_floating_point<T>::value;


	//判断是不是算数类型
	template<class T>
		static constexpr bool is_arithmetic_v = is_integral_v<T> || is_floating_point_v<T>;

	//判断是不是array
	template<class T>
		struct is_array
		{
			static constexpr bool value = false;
		};

	template<class T,size_t N>
		struct is_array<T[N]>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_array<T[]>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_array_v = is_array<T>::value;

	//判断是不是reference
	template<class T>
		struct is_reference
		{
			static constexpr bool value = false;
		};

	template<class T>
		struct is_reference<T&>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_reference<T&&>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_reference<const T&>
		{
			static constexpr bool value = true;
		};

	template<class T>
		struct is_reference<const T&&>
		{
			static constexpr bool value = true;
		};

	template<class T>
		static constexpr bool is_reference_v = is_reference<T>::value;

	//判断是不是成员函数指针
	template<class T>
		struct is_member_pointer
		{
			static constexpr bool value = false;
		};

	template<class T,class U>
		struct is_member_pointer<T U::*>
		{
			static constexpr bool value = true;
		};

	template <typename T>
		static constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

	//判断是不是标量类型，即除用户自定义类型和void类型之外的内置类型
	//少了一个枚举类型，不会写，寄！
	template<class T>
		static constexpr bool is_scalar_v = is_arithmetic_v<T> || is_pointer_v<T> || is_member_pointer_v<T>; 

	//C++标准中定义了四种标量类型（scalar type）：算术类型（arithmetic types）、枚举类型（enumeration types）、指针类型（pointer types）和成员指针类型（pointer-to-member types）。其中，除了枚举类型以外，其他三种标量类型都可以是POD类型。
	//C++标准还定义了一些复合类型（compound types），例如数组类型（array types）、结构体类型（struct types）、联合类型（union types）和类类型（class types）。这些类型中，只有标准布局类型（standard layout types）才能是POD类型。标准布局类型具有以下特性：

	//所有非静态数据成员都有相同的访问控制（public、private或protected）和非虚函数访问属性（非静态数据成员不能是volatile或mutable的）。
	//所有非静态数据成员都是相同的类型或都是标准布局类型的非静态数据成员。
	//如果有一个非静态数据成员的类型是非引用类型的非联合类型，则第一个非静态数据成员与其类型具有相同的访问控制和非虚函数访问属性。
	//类中最多只有一个非静态数据成员是联合类型。
	//类没有虚函数和虚基类。
	//因此，对于复合类型，只有符合标准布局的类型才能是POD类型。此外，C++标准还定义了一些语言特性，例如虚函数、多重继承和非平凡的构造函数等，这些特性都可能导致类型不再是POD类型。因此，在使用std::is_pod和std::is_pod_v时，需要仔细分析和验证类型是否符合POD类型的要求。
	//

	//这个我没能力实现。。。。。。
	template<class T>
		struct is_standard_layout
		{
			static constexpr bool value = false;
		};

	template<class T>
		static constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

	//这个也没能力实现。。。。。。
	template<class T>
		struct is_trivial
		{
			static constexpr bool value = false;
		};

	template<class T>
		static constexpr bool is_trivial_v = is_trivial<T>::value;

	//我目前唯一能做到的只能把一些内置类型当做pod类型。。。。。。
	//我知道pod类型需要 is_trivial_v && is_standard_layout
	//但是鉴于目前实现不了那两个重要编译期谓词所有只能将就了。。。
	template<class T>
		static constexpr bool is_pod_v = is_arithmetic_v<T> || is_pointer_v<T> || is_member_pointer_v<T>;


	//remove_reference
	//std::remove_reference 是一个 C++11 中引入的类型转换辅助工具，用于从一个类型中移除引用限定符，返回对应的非引用类型。
	//该实现使用了模板特化技术，分别处理了三种情况：
	//当传入的类型 T 不是引用类型时，直接返回 T。
	//当传入的类型 T 是左值引用类型 T& 时，返回对应的 T 类型。
	//当传入的类型 T 是右值引用类型 T&& 时，返回对应的 T 类型。
	template <typename T>
		struct remove_reference 
		{
			using type = T;
		};

	template <typename T>
		struct remove_reference<T&> 
		{
			using type = T;
		};

	template <typename T>
		struct remove_reference<T&&> 
		{
			using type = T;
		};


	//还有很多的type_traits就不一一写了......


};
#endif

