#ifndef _ITERATOR_TRAITS_H_
#define _ITERATOR_TRAITS_H_
namespace my_stl
{

	//迭代器需要支持*，++，--等操作
	//下面是五种迭代器标签类型

	//input_iterator_tag 可以包装函数的输出——以用作它人的输入流。所以它是仅可递增的（只能 +1），你不能对它 +n，只能通过循环 n 次递增来模拟相应的效果。input_iterator 无法递减（-1），因为输入流没有这样的特性。它的迭代器值（*it）是只读的，你不能对其置值。
	struct input_iterator_tag{};

	//output_iterator 没有 ++/-- 能力。你可以向 output_iterator 指向的容器中写入/置入新值，仅此而已
	struct output_iterator_tag {};

	//forward_iterator_tag 表示前向迭代器，所以只能增量++，不能回退，它继承 input_iterator_tag 的一切基本能力，但又有所增强，例如允许设置值。
	struct forward_iterator_tag : public input_iterator_tag{};

	//bidirectional_iterator_tag 的关联实体 bidirectional_iterator 是双向可行走的，既可以 it++  也可以 it--，例如 std::list。如同 forward_iterator_tag 一样，bidirectional_iterator_tag 不能直接 +n （和 -n），所以 +n 需要一个特化的 advance 函数来循环 n 次，每次 +1（即通过循环 n 次递增或递减来模拟）。
	struct bidirectional_iterator_tag : public forward_iterator_tag{};

	//random_access_iterator_tag 表示的随机访问迭代器，random_access_iterator 支持读取/设置值，支持递增递减，支持 +n/-n。
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};


	//迭代器的五个接口！！！
	//value_type 表示迭代器所指的类型
	//pointer 表示迭代器所指类型的指针
	//distance_type 表示两个迭代器之间的距离
	//reference 表示迭代器所指类型的引用
	//iterator_category 表示迭代器的类型是哪一种迭代器便签
	
	//迭代器萃取类，萃取出迭代器的所有特性
	template<class Iterator>
		struct iterator_traits
		{
			using value_type = typename Iterator::value_type;
			using distance_type = typename Iterator::distance_type;
			using pointer = typename Iterator::pointer;
			using reference = typename Iterator::reference;
			using iterator_category = typename Iterator::iterator_category;
		};


	//几个基本的迭代器判断编译期谓词
	
	//is_random_access_iterator 
	template<class Iterator>
		struct is_random_access_iterator
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_random_access_iterator<random_access_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_random_access_iterator<const random_access_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<class Iterator>
		static constexpr bool is_random_access_iterator_v = is_random_access_iterator<Iterator>::value;

	//is_input_iterator    
	template<class Iterator>
		struct is_input_iterator
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_input_iterator<input_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_input_iterator<const input_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<class Iterator>
		static constexpr bool is_input_iterator_v = is_input_iterator<Iterator>::value;

	//is_bidirectional_iterator
	template<class Iterator>
		struct is_bidirectional_iterator
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_bidirectional_iterator<const bidirectional_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_bidirectional_iterator<bidirectional_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<class Iterator>
		static constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;

	//is_forward_iterator
	template<class Iterator>
		struct is_forward_iterator
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_forward_iterator<forward_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_forward_iterator<const forward_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<class Iterator>
		static constexpr bool is_forward_iterator_v = is_forward_iterator<Iterator>::value;

	//is_output_iterator
	template<class Iterator>
		struct is_output_iterator
		{
			static constexpr bool value = false;
		};

	template<>
		struct is_output_iterator<output_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<>
		struct is_output_iterator<const output_iterator_tag>
		{
			static constexpr bool value = true;
		};

	template<class Iterator>
		static constexpr bool is_output_iterator_v = is_output_iterator<Iterator>::value;

	//针对指针T*特化
	template<class Iterator>
		struct iterator_traits<Iterator*>
		{
			using value_type = Iterator;
			using distance_type = ptrdiff_t;
			using pointer = Iterator*;
			using reference = Iterator&;
			using iterator_category = random_access_iterator_tag;
		};

	//针对指针const T*特化
	template<class Iterator>
		struct iterator_traits<const Iterator*>
		{
			using value_type = Iterator;
			using distance_type = ptrdiff_t;
			using pointer = const Iterator*;
			using reference = const Iterator&;
			using iterator_category = random_access_iterator_tag;
		};

};
#endif
