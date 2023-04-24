#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include"iterator_traits.h"
#include"type_traits.h"
#include"functional.h"
#include"memory.h"
namespace my_stl
{
	//pair类型结构体，存放键值对
	template<class Key,class Value> 
		struct pair;

	//distance
	//根据迭代器类型，采取不同方法获取两个迭代器之间的距离
	template<class InputIterator>
		inline  typename iterator_traits<InputIterator>::distance_type distance(InputIterator first,InputIterator last) noexcept
		{
			using _itertype = typename iterator_traits<InputIterator>::iterator_category;
			using _dtype = typename iterator_traits<InputIterator>::distance_type;
			//随机访问迭代器类型直接相减
			if constexpr(is_random_access_iterator_v<_itertype>)
			{
				return static_cast<_dtype>(last - first);
			}
			else
			{
				//其他类型通过循环相加
				_dtype n = 0;
				for(;first != last;++first,++n);
				return n;
			}
		}

	//is_sorted
	//判断范围内的元素是不是按照二元谓词op排序
	template<class InputIterator,class Op>
		inline bool is_sorted(InputIterator first,InputIterator last,const Op& op) noexcept
		{
			if(first == last)
			{
				return true;
			}
			auto next = first;
			while(++next != last)
			{
				if(op(*next,*first))
				{
					return false;
				}
				++first;
			}
			return true;
		}

	//判断是不是升序
	//使用less
	template<class InputIterator>
		inline bool is_sorted_up(InputIterator first,InputIterator last) noexcept
		{
			return is_sorted(first,last,less<typename iterator_traits<InputIterator>::value_type>());
		}

	//判断是不是降序
	//使用greater
	template<class InputIterator>
		inline bool is_sorted_down(InputIterator first,InputIterator last) noexcept
		{
			return is_sorted(first,last,greater<typename iterator_traits<InputIterator>::value_type>());
		}

	//merge_sort
	//将范围内的元素按照二元谓词op进行归并排序
	template<class RandomAccessIterator,class Comp,class T,class Size>
		inline void _merge_sort_aux(Size first,Size last,RandomAccessIterator array,const Comp& op,T arrtmp) noexcept
		{
			if(first >= last)
			{
				return;
			}
			Size mid = first + (last - first) / 2;
			Size left_begin = first,left_end = mid,right_begin = mid + 1,right_end = last;
			_merge_sort_aux(left_begin,left_end,array,op,arrtmp);
			_merge_sort_aux(right_begin,right_end,array,op,arrtmp);
			Size ii = first;
			while(left_begin <= left_end && right_begin <= right_end)
			{
				arrtmp[ii++] = op(array[left_begin],array[right_begin]) ? array[left_begin++] : array[right_begin++];
			}
			while(left_begin <= left_end)
			{
				arrtmp[ii++] = array[left_begin++];
			}
			while(right_begin <= right_end)
			{
				arrtmp[ii++] = array[right_begin++];
			}
			for(;first <= last;++first)
			{
				array[first] = arrtmp[first];
			}
		}

	template<class RandomAccessIterator,class Comp,class Size>
		inline void _merge_sort_(Size first,Size last,RandomAccessIterator array,const Comp& op) noexcept
		{
			using _ptype = typename iterator_traits<RandomAccessIterator>::pointer;
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			_ptype arrtmp = static_cast<_ptype>(malloc((last - first) * sizeof(_valtype)));
			if(arrtmp == nullptr)
			{
				return;
			}
			_merge_sort_aux(first,last - 1,array,op,arrtmp);
			destroy(arrtmp,arrtmp + (last - first));
			free(arrtmp);
			arrtmp = nullptr;
		}

	//merge_sort
	//使用二元谓词op进行排序
	template<class RandomAccessIterator,class Comp>
		inline void merge_sort(RandomAccessIterator first,RandomAccessIterator last,const Comp& op) noexcept	
		{
			static_assert(is_same_v<typename iterator_traits<RandomAccessIterator>::iterator_category,random_access_iterator_tag>);
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			_merge_sort_(_dtype(0),_dtype(last - first),first,op);
		}

	//merge sort
	//默认进行升序排序
	template<class RandomAccessIterator>
		inline void merge_sort(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			static_assert(is_random_access_iterator_v<typename iterator_traits<RandomAccessIterator>::iterator_category>);
			_merge_sort_(_dtype(0),_dtype(last - first),first,less<typename iterator_traits<RandomAccessIterator>::value_type>());
		}

	//quick sort
	//快速排序
	template<class ForwardIterator,class Comp>
		void _sort_quick_sort(ForwardIterator first,ForwardIterator last,const Comp& comp) noexcept
		{
			using _dtype =  typename iterator_traits<ForwardIterator>::distance_type;
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			_dtype n = distance(first,last);
			if(n < 2)
			{
				return;
			}
			_valtype tmp = *first;
			ForwardIterator left = first,right = last;
			--right;
			bool flag = true;
			while(left != right)
			{
				if(flag)
				{
					if(comp(tmp,*right))
					{
						--right;
						continue;
					}
					*left = *right;
					++left;
					flag = false;
				}
				else
				{
					if(comp(*left,tmp))
					{
						++left;
						continue;
					}
					*right = *left;
					--right;
					flag = true;
				}
			}
			if(left != first)
			{
				*left = tmp;
			}
			_sort_quick_sort(first,left,comp);
			_sort_quick_sort(++left,last,comp);
		}


	//insert sort
	template<class RandomAccessIterator,class T>
		inline void __unguarded_linear_insert(RandomAccessIterator last,T value) noexcept
		{
			RandomAccessIterator next = last;
			--next;
			while(value < *next)
			{
				*last = *next;
				last = next;
				--next;
			}
			*last = value;
		}

	//insert sort
	template<class RandomAccessIterator>
		inline void __insert_sort(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			_valtype value = *last;
			if(value < *first)
			{
				copy_backward(first,last,last + 1);
				*first = value;
			}
			else
			{
				__unguarded_linear_insert(last,value);
			}
		}

	//insert sort
	template<class RandomAccessIterator>
		inline void _insert_sort(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			if(first == last)
			{
				return;
			}
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			if constexpr(is_random_access_iterator_v<_itertype>)
			{
				for(auto i = first + 1;i != last;++i)
				{
					__insert_sort(first,i);
				}
			}
		}

	//sort
	//使用自定义二元谓词comp进行排序
	template<class ForwardIterator,class Comp>
		inline void sort(ForwardIterator first,ForwardIterator last,const Comp& comp) noexcept
		{
			using _itertype = typename iterator_traits<ForwardIterator>::iterator_category;
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_random_access_iterator_v<_itertype> || is_bidirectional_iterator_v<_itertype>)
			{
				_sort_quick_sort(first,last,comp);
			}
			else
			{
			}
		}

	//sort
	//默认使用less升序排序
	template<class ForwardIterator>
		inline void sort(ForwardIterator first,ForwardIterator last) noexcept
		{
			using _itertype = typename iterator_traits<ForwardIterator>::iterator_category;
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_random_access_iterator_v<_itertype> || is_bidirectional_iterator_v<_itertype>)
			{
				_sort_quick_sort(first,last,less<_valtype>());
			}
			else 
			{

			}
		}

	//for_each 
	//对每一个元素使用op操作
	template<class ForwardIterator,class Op>
		inline void for_each(ForwardIterator first,ForwardIterator last,const Op& op) noexcept
		{
			for(;first != last;++first)
			{
				op(*first);
			}
		}

	//transform
	//对每一个元素使用op操作,然后将结果写到dest开始的地方
	template<class InputIterator,class ForwardIterator,class Op>
		inline ForwardIterator transform(InputIterator first,InputIterator last,ForwardIterator dest,const Op& op) noexcept
		{
			for(;first != last;++first,++dest)
			{
				*dest = op(*first);
			}
			return dest;
		}

	//计算数组大小函数array_size
	template<class T,size_t N>
		inline constexpr size_t array_size(const T(&)[N]) noexcept
		{
			return N;
		}

	//count_if函数
	//将指定操作（一个仿函数）pred实施于[first,last)区间内的每一个元素身上，并将造成pred计算结果为true的所有元素个数返回。
	template<class IuputIterator,class Op>
		inline typename iterator_traits<IuputIterator>::distance_type count_if(IuputIterator first,IuputIterator last,const Op& op) noexcept
		{
			using _dtype = typename iterator_traits<IuputIterator>::distance_type;
			_dtype n = 0;
			for(;first != last;++first)
			{
				if(op(*first))
				{
					++n;
				}
			}
			return n;
		}

	//count函数
	//使用operator=来进行默认比较
	template<class IuputIterator,class T>
		inline typename iterator_traits<IuputIterator>::distance_type count(IuputIterator first,IuputIterator last,const T& val) noexcept
		{
			return count_if(first,last,[&](const T& v){return v == val;});
		}

	//递增或递减i步的迭代器
	template<class Iterator,class Size>
		inline void advance(Iterator& iter,Size i) noexcept
		{
			using _itertype = iterator_traits<Iterator>::iterator_category;
			if constexpr(is_random_access_iterator_v<_itertype>)
			{
				iter += i;
			}
			else if constexpr(is_bidirectional_iterator_v<_itertype>)
			{
				if(i >= 0)
				{
					while(i--){++iter;}
				}
				else
				{
					while(i++ < 0){--iter;}
				}
			}
			else if constexpr(is_forward_iterator_v<_itertype> || is_input_iterator_v<_itertype>)
			{
				if(i < 0)
				{
					return;
				}
				while(i--){++iter;}
			}
		}

	//lower_bound试图在已排序的区间[first,last)中寻找元素value。如果区间中存在该元素，则返回一个迭代器指向其中第一个元素。如果没有找到该元素，则返回 “假设该元素存在时应该出现的位置” 的迭代器。也就是说会返回一个迭代器指向第一个不大于value的元素。如果value大于区间内所有的元素，则返回last。
	template<class InputIterator,class T,class Comp>
		inline InputIterator lower_bound(InputIterator first,InputIterator last,const T& val,const Comp& comp) noexcept
		{
			using _dtype = typename iterator_traits<InputIterator>::distance_type;
			_dtype len = distance(first,last),half;
			InputIterator middle;
			while(len > 0)
			{
				half = len >> 1;
				middle = first;
				advance(middle,half);
				if(comp(*middle,val))
				{
					first = middle;
					++first;
					len = len - half - 1;
				}
				else
				{
					len = half;
				}
			}
			return first;
		}

	template<class InputIterator,class T>
		inline InputIterator lower_bound(InputIterator first,InputIterator last,const T& val) noexcept
		{
			using _valtype = typename iterator_traits<InputIterator>::value_type;
			return lower_bound(first,last,val,less<_valtype>());
		}


	//lower_bound试图在已排序的区间[first,last)中寻找元素value。如果区间中存在该元素，则返回一个迭代器指向其中第一个元素。如果没有找到该元素，则返回 “假设该元素存在时应该出现的位置” 的迭代器。也就是说会返回一个迭代器指向第一个不大于value的元素。如果value大于区间内所有的元素，则返回last。
	template<class InputIterator,class T,class Comp>
		inline InputIterator upper_bound(InputIterator first,InputIterator last,const T& val,const Comp& comp) noexcept
		{
			using _dtype = typename iterator_traits<InputIterator>::distance_type;
			_dtype len = distance(first,last),half;
			InputIterator middle;
			while(len > 0)
			{
				half = len >> 1;
				middle = first;
				advance(middle,half);
				if(comp(val,*middle))
				{
					len = half;
				}
				else
				{
					first = middle;
					++first;
					len = len - half - 1;
				}
			}
			return first;
		}

	template<class InputIterator,class T>
		inline InputIterator upper_bound(InputIterator first,InputIterator last,const T& val) noexcept
		{

			using _valtype = typename iterator_traits<InputIterator>::value_type;
			return upper_bound(first,last,val,less<_valtype>());
		}

	//binary_search是最简单的二分查找，它的返回值是bool，无法提供能够利用的信息，如该元素的索引位置，它只能告诉你该元素是否存在！！！
	template<class InputIterator,class T>
		inline bool binary_search(InputIterator first,InputIterator last,const T& val) noexcept
		{
			auto i = lower_bound(first,last,val);
			return i != last && !(val < *i);
		}

	//equal_range也是二分查找的一种。该算法试图在已排序区间[first,last)中寻找value。返回值是一个pair类型，first和second都是迭代器。其中first的lower_bound的结果，second是upper_bound的结果！！！如果区间内没有value元素，这种情况下返回的两个迭代器形成了一个空区间。这个条件非常重要，因为可以用来进行精确的二分查找了！！！
	template<class InputIterator,class T>
		inline const pair<InputIterator,InputIterator> equal_range(InputIterator first,InputIterator last,const T& val) noexcept
		{
			using _dtype = typename iterator_traits<InputIterator>::distance_type;
			_dtype len = distance(first,last),half;
			InputIterator middle,left,right;
			while(len > 0)
			{
				half = len >> 1;
				middle = first;
				advance(middle,half);
				if(*middle < val)
				{
					first = middle;
					++first;
					len = len - half - 1;
				}
				else if(val < *middle)
				{
					len = half;
				}
				else
				{
					left = lower_bound(first,middle,val);
					advance(first,len);
					right = upper_bound(++middle,first,val);
					return pair<InputIterator,InputIterator>(left,right);
				}
			}
			return pair<InputIterator,InputIterator>(first,first);
		}

	//find根据operator=运算符，顺序查找[first,last)区间内的每一个元素，找出第一个匹配的元素。如果找到就返回一个InputIterator指向该元素，否则返回last。
	template<class InputIterator,class T>
		inline  InputIterator find(InputIterator first,InputIterator last,const T& val) noexcept
		{
			for(;first != last;++first)
			{
				if(*first == val)
				{
					return first;
				}
			}
			return last;
		}

	//find_if根据指定的pred运算条件（以仿函数表示），顺序查找[first,last)区间内的每一个元素，找出第一个使pred为ture的元素。如果找到就返回一个InputIterator指向该元素，否则返回last。
	template<class InputIterator,class Comp>
		inline InputIterator find_if(InputIterator first,InputIterator last,const Comp& op)
		{
			for(;first != last; ++first)
			{
				if(op(*first))
				{
					return first;
				}
			}
			return last;
		}

	//find_first_of用于查找容器内子序列的第一次出现。它在范围[first1，last1)中搜索由[first2，last2)定义的序列的第一次出现，然后将迭代器返回到其第一个元素，如果没有发现，则返回last1。
	template<class InputIterator1,class InputIterator2>
		inline bool _find_first_of(InputIterator1 first1,InputIterator2 first2,InputIterator2 last2) noexcept
		{
			for(++first1,++first2;first2 != last2;++first2,++first1)
			{
				if(*first1 != *first2)
				{
					return false;
				}
			}
			return true;
		}

	//find_first_of
	template<class InputIterator1,class InputIterator2>
		inline InputIterator1 find_first_of(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2) noexcept
		{
			for(;first1 != last1;++first1)
			{
				if(*first1 == *first2)
				{
					if(_find_first_of(first1,first2,last2))
					{
						return first1;
					}
				}
			}
			return last1;
		}

	//adjacent_find
	//该算法找出第一组满足条件的相邻元素。这里所谓的条件，在版本一中是指两个相邻元素相等；在版本二中允许用户指定一个二元运算，两个操作数分别是相邻的第一个和第二个元素。该算法会从first开始遍历到last，时间复杂度为O（logn）。
	template<class ForwardIterator>
		inline ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last) noexcept
		{
			if(first == last)
			{
				return last;
			}
			ForwardIterator next = first;
			for(;++next != last;)
			{
				if(*first == *next)
				{
					return first;
				}
				first = next;
			}
			return last;
		}

	//另一个版本的adjacent_find
	template<class ForwardIterator,class Op>
		inline ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last,const Op& op) noexcept
		{
			if(first == last)
			{
				return last;
			}
			ForwardIterator next = first;
			for(;++next != last;)
			{
				if(op(*first,*next))
				{
					return first;
				}
				first = next;
			}
			return last;
		}

	//print
	//格式化输出函数
	template<class...Args>
		inline void print(const Args&... args) noexcept
		{
			((std::cout << args),...);
		}

	//pair结构体
	template<class Key,class Value>
		struct pair
		{
			//键值对
			Key first;
			Value second;

			pair() :first(),second(){}

			pair(const Key& k,const Value& v) noexcept:first(k),second(v) {}

			pair(const pair& p) noexcept:first(p.first),second(p.second) {}

			~pair() noexcept {}

			pair& operator =(const pair& rhs) noexcept
			{
				if(this != &rhs)
				{
					first = rhs.first;
					second = rhs.second;
				}
				return *this;
			}

			inline bool operator ==(const Key& k) const noexcept
			{
				return k == first;
			}

			inline bool operator !=(const Key& k) const noexcept
			{
				return k != first;
			}

			inline bool operator ==(const pair& p) const noexcept
			{
				return p.first == first;
			}

			inline bool operator !=(const pair& p) const noexcept
			{
				return p.first != first;
			}
		};

	//swap操作，交换两个迭代器的value值
	template<class OuputIterator>
		inline void _iterator_value_swap(OuputIterator lhs,OuputIterator rhs) noexcept
		{
			using _valtype = typename iterator_traits<OuputIterator>::value_type;
			_valtype tmp = *lhs;
			*lhs = *rhs;
			*rhs = tmp;
		}

	//泛型swap操作
	template<class T>
		inline void swap(T& lhs,T& rhs) noexcept
		{
			T tmp = lhs;
			lhs = rhs;
			rhs = tmp;
		}

	//partition
	//partition会将区间[first,last)中的元素重新排列。所有被一元条件运算pred判定为true的元素都会被放在区间的前段；被判定为false的元素都会被放在区间的后段。这个算法并不保证保留元素的原始相对位置。如果需要保留原始相对位置，应该使用stable_partition。
	template<class BidirectionalIterator,class Op>
		inline BidirectionalIterator partition(BidirectionalIterator first,BidirectionalIterator last,const Op& op) noexcept
		{
			using _itertype = typename iterator_traits<BidirectionalIterator>::iterator_category;
			static_assert(is_bidirectional_iterator_v<_itertype>);
			while(true)
			{
				while(true)
				{
					if(first == last)
					{
						return first;
					}
					else if(op(*first))
					{
						++first;
					}
					else
					{
						break;
					}
				}
				--last;
				while(true)
				{
					if(first == last)
					{
						return first;
					}
					else if(!op(*last))
					{
						--last;
					}
					else
					{
						break;
					}
				}
				_iterator_value_swap(first,last);
				++first;
			}
		}

	//这个replace系列的函数将[first,last)区间内所有与old_value相等的值用new_value来替换。有接受一元谓词的if版本，有重定位输出的copy版本。
	template<class ForwardIterator,class T>
		inline void replace(ForwardIterator first,ForwardIterator last,const T& old_val,const T& new_val) noexcept
		{
			for(;first != last;++first)
			{
				if(*first == old_val)
				{
					*first = new_val;
				}
			}
		}

	template<class ForwardIterator,class Op,class T>
		inline void replace_if(ForwardIterator first,ForwardIterator last,const Op& op,const T& val) noexcept
		{
			for(;first != last;++first)
			{
				if(op(*first))
				{
					*first = val;
				}
			}
		}

	template<class ForwardIterator,class OuputIterator,class T>
		inline void replace_copy(ForwardIterator first,ForwardIterator last,OuputIterator dest,const T& old_val,const T& new_val) noexcept
		{
			for(;first != last;++first,++dest)
			{
				*dest = *first == old_val ? new_val : *first;
			}
			return dest;
		}

	template<class ForwardIterator,class OuputIterator,class Op,class T>
		inline void replace_copy_if(ForwardIterator first,ForwardIterator last,OuputIterator dest,const Op& op,const T& val) noexcept
		{
			for(;first != last;++first,++dest)
			{
				*dest = op(*first) ? val : *first;
			}
			return dest;
		}

	//reverse
	//这两个算法将序列[first,last)内的元素颠倒重排。copy版本的重定位了输出，且对原容器的元素不会造成改变。
	template<class BidirectionalIterator>
		inline void reverse(BidirectionalIterator first,BidirectionalIterator last) noexcept
		{
			using _itertype = typename iterator_traits<BidirectionalIterator>::iterator_category;
			if constexpr(is_bidirectional_iterator_v<_itertype>)
			{
				while(true)
				{
					if(first == last || first == --last)
					{
						return;
					}
					else
					{
						_iterator_value_swap(first++,last);
					}
				}
			}
			else if constexpr(is_random_access_iterator_v<_itertype>)
			{
				while(first < last)
				{
					_iterator_value_swap(first++,--last);
				}
			}
		}

	//reverse_copy
	template<class BidirectionalIterator,class ForwardIterator>
		inline void reverse_copy(BidirectionalIterator first,BidirectionalIterator last,ForwardIterator dest) noexcept
		{
			using _itertype = typename iterator_traits<BidirectionalIterator>::iterator_category;
			if constexpr(is_random_access_iterator_v<_itertype> || is_bidirectional_iterator_v<_itertype>)
			{
				while(first != last)
				{
					--last;
					*dest = *last;
					++dest;
				}
				return dest;
			}
			else
			{
				return dest;
			}
		}

	//remove （移除但不删除）
	//注意这一法则对于vector，array成立，对于list不成立！！！！
//移除[first,last)之中所有与value相等的元素。这一算法并不真正从容器中删除哪些元素（换句话说容器的size并没有改变），而是将每一个不与value相等的元素轮番赋值给first之后的空间。返回值ForwardIterator标示出重新整理后的最后元素的下一个位置。这时可以利用成员函数erase删除掉该位置到尾迭代器位置的数据，这样的话就是真正的删除了。也提供了使用一元谓词的版本。注意这个算法不适用array，因为array无法缩小尺寸，导致残余数据永远存在。
	//例如{0,1,0,2,0,3,0,4}需要移除0变成{1,2,3,4,0,3,0,4}
	template<class InputIterator,class OuputIterator,class T>
		inline OuputIterator remove_copy(InputIterator first,InputIterator last,OuputIterator dest,const T& val) noexcept
		{
			for(;first != last;++first)
			{
				if(*first != val)
				{
					*dest = *first;
					++dest;
				}
			}
			return dest;
		}

	template<class ForwardIterator,class T>
		inline ForwardIterator remove(ForwardIterator first,ForwardIterator last,const T& val) noexcept
		{
			first = find(first,last,val);
			ForwardIterator next = first;
			return first == last ? first :remove_copy(++next,last,first,val);
		}

	template<class InputIterator,class Op,class OuputIterator>
		inline OuputIterator remove_copy_if(InputIterator first,InputIterator last,OuputIterator dest,const Op& op) noexcept
		{
			for(;first != last;++first)
			{
				if(!op(*first))
				{
					*dest = *first;
					++dest;
				}
			}
			return dest;
		}

	template<class ForwardIterator,class Op>
		inline ForwardIterator remove_if(ForwardIterator first,ForwardIterator last,const Op& op) noexcept
		{
			first = find_if(first,last,op);
			ForwardIterator next = first;
			return first == last ? first : remove_copy_if(++next,last,first,op);
		}

	//unique
	//unique能够移除重复的元素，每当在[first,lasy)中遇到重复元素时，它便移除重复元素群中第一个后的所有元素。注意unique只移除相邻的重复元素，如果想移除所有包括不相邻的重复元素，必须先将序列排序，使所有重复元素相邻
	template<class InputIterator,class ForwardIterator,class Op>
		inline ForwardIterator unique_copy_if(InputIterator first,InputIterator last,ForwardIterator dest,const Op& op) noexcept
		{
			if(first == last)
			{
				return dest;
			}
			using _itertype = typename iterator_traits<ForwardIterator>::iterator_category;
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			if constexpr(is_forward_iterator_v<_itertype> || is_random_access_iterator_v<_itertype> || is_bidirectional_iterator_v<_itertype> || is_input_iterator_v<_itertype>)
			{
				*dest = *first;
				while(++first != last)
				{
					if(!op(*first,*dest))
					{
						*++dest = *first;
					}
				}
				return ++dest;
			}
			else
			{
				_valtype value = *first;
				*dest = value;
				while(++first != last)
				{
					if(!op(value,*first))
					{
						value = *first;
						*++dest = value;
					}
				}
				return ++dest;
			}
		}

	template<class ForwardIterator,class Op>
		inline ForwardIterator unique_if(ForwardIterator first,ForwardIterator last,const Op& op) noexcept
		{
			first = adjacent_find(first,last,op);
			return unique_copy_if(first,last,first,op);
		}

	template<class ForwardIterator>
		inline ForwardIterator unique(ForwardIterator first,ForwardIterator last) noexcept
		{
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			first = adjacent_find(first,last);
			return unique_copy_if(first,last,first,equal_to<_valtype>());
		}

	template<class InputIterator,class ForwardIterator>
		inline ForwardIterator unique_copy(InputIterator first,InputIterator last,ForwardIterator dest) noexcept
		{
			using _valtype = typename iterator_traits<ForwardIterator>::value_type;
			first = adjacent_find(first,last);
			return unique_copy_if(first,last,dest,equal_to<_valtype>());
		}


	//heap algorithm
	//堆算法，要求是随机访问迭代器！！！！
	//核心操作，元素下沉！！！
	template<class RandomAccessIterator,class Distance,class Comp>
		void _heapify_(RandomAccessIterator first,Distance parent,Distance end,const Comp& comp) noexcept
		{
			Distance max_or_min_ldx_ = parent;
			Distance left_child = parent * 2 + 1;
			Distance right_child = left_child + 1;
			if(left_child < end && comp(first[max_or_min_ldx_],first[left_child]))
			{
				max_or_min_ldx_ = left_child;
			}
			if(right_child < end && comp(first[max_or_min_ldx_],first[right_child]))
			{
				max_or_min_ldx_ = right_child;
			}
			if(max_or_min_ldx_ != parent)
			{
				swap(first[max_or_min_ldx_],first[parent]);
				_heapify_(first,max_or_min_ldx_,end,comp);
			}
		}

	//可以自己指定生成最大堆或者最小堆
	template<class RandomAccessIterator,class Comp>
		inline void make_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			static_assert(is_random_access_iterator_v<_itertype>);
			_dtype size = last - first;
			for(_dtype i = size / 2 - 1;i >= 0;--i)
			{
				_heapify_(first,i,size,comp);
			}
		}

	//默认制作一个最大堆
	template<class RandomAccessIterator>
		inline void make_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			make_heap(first,last,less<_valtype>());
		}

	//堆排序
	template<class RandomAccessIterator,class Comp>
		inline void sort_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			static_assert(is_random_access_iterator_v<_itertype>);
			_dtype size = distance(first,last);
			if(size < 2)
			{
				return;
			}
			for(_dtype i = size - 1;i > 0;--i)
			{
				swap(first[i],first[0]);
				_heapify_(first,_dtype(0),i,comp);
			}
		}

	//堆排序默认生成最大堆来进行升序排序
	template<class RandomAccessIterator>
		inline void sort_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			sort_heap(first,last,less<_valtype>());
		}

	//判断一个堆是不是最大堆
	//使用了less
	template<class RandomAccessIterator,class Comp>
		inline bool is_max_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			static_assert(is_random_access_iterator_v<_itertype>);
			_dtype n = last - first,left_child = 0,right_child = 0;
			for(_dtype i = 0;i < n;++i)
			{
				left_child = 2 * i + 1;
				right_child = left_child + 1;
				if(left_child < n && comp(first[i],first[left_child]))
				{
					return false;
				}
				if(right_child < n && comp(first[i],first[right_child]))
				{
					return false;
				}
			}
			return true;
		}

	//判断一个堆是不是最大堆
	//使用了less
	template<class RandomAccessIterator>
		inline bool is_max_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			return is_max_heap(first,last,less<_valtype>());
		}

	//判断一个堆是不是最小堆
	//使用了greater
	template<class RandomAccessIterator,class Comp>
		inline bool is_minimum_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			static_assert(is_random_access_iterator_v<_itertype>);
			_dtype n = last - first,left_child = 0,right_child = 0;
			for(_dtype i = 0;i < n;++i)
			{
				left_child = 2 * i + 1;
				right_child = left_child + 1;
				if(left_child < n && comp(first[i],first[left_child]))
				{
					return false;
				}
				if(right_child < n && comp(first[i],first[right_child]))
				{
					return false;
				}
			}
			return true;
		}

	//判断一个堆是不是最小堆
	//使用了greater
	template<class RandomAccessIterator>
		inline bool is_minimum_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			return is_minimum_heap(first,last,greater<_valtype>());
		}

	//push_heap
	template<class RandomAccessIterator>
		inline void push_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			make_heap(first,last);
		}

	//push_heap
	//将一个元素插入到堆中，并保持堆的性质不变。它的作用是将新元素插入到堆的末尾，然后将该元素上浮到合适的位置，使得堆仍然满足堆的性质
	template<class RandomAccessIterator,class Comp>
		inline void push_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			make_heap(first,last,comp);
		}

	//pop_heap
	//用于将堆的根节点移除，并将堆的最后一个元素放到根节点位置，然后重新构建堆。它的作用是将堆中最大（或最小）的元素移动到堆的末尾，使堆的大小减小1，并保持堆的性质不变。
	template<class RandomAccessIterator,class Comp>
		inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last,const Comp& comp) noexcept
		{
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			static_assert(is_random_access_iterator_v<_itertype>);
			swap(*first,*(last - 1));
			make_heap(first,last - 1,comp);
		}

	//pop_heap
	template<class RandomAccessIterator>
		inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			using _valtype = typename iterator_traits<RandomAccessIterator>::value_type;
			pop_heap(first,last,less<_valtype>());
		}

	//accumulate
	//累加求和
	template<class InputIterator,class T>
		inline T accumulate(InputIterator first,InputIterator last,T init) noexcept
		{
			for(;first != last;++first)
			{
				init += *first;
			}
			return init;
		}

	//accumulate
	template<class InputIterator,class T,class Op>
		inline T accumulate(InputIterator first,InputIterator last,T init,const Op& op) noexcept
		{
			for(;first != last;++first)
			{
				init = op(init,*first);
			}
			return init;
		}

	//equal
	//判断两个区间是否相等
	template<class InputIterator1,class InputIterator2>
		inline bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2) noexcept
		{
			if(distance(first1,last1) != distance(first2,last2))
			{
				return false;
			}
			for(;first1 != last1;++first1,++first2)
			{
				if(*first1 != *first2)
				{
					return false;
				}
			}
			return true;
		}

	//equal
	template<class InputIterator1,class InputIterator2,class Comp>
		inline bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,const Comp& comp) noexcept
		{
			if(distance(first1,last1) != distance(first2,last2))
			{
				return false;
			}
			for(;first1 != last1;++first1,++first2)
			{
				if(!comp(*first2,*first1))
				{
					return false;
				}
			}
			return true;
		}

	//max
	template<class T>
		inline T max(const T& x,const T& y) noexcept
		{
			return x < y ? y : x;
		}

	//min
	template<class T>
		inline T min(const T& x,const T& y) noexcept
		{
			return x < y ? x : y;
		}

	//mismatch
	//会找出两个区间第一个不匹配的点，返回一对迭代器
	template<class InputIterator1,class InputIterator2>
		pair<InputIterator1,InputIterator2> mismatch(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2) noexcept
		{
			while(first1 != last1 && *first1 == *first2)
			{
				++first1;++first2;
			}
			return pair<InputIterator1,InputIterator2>(first1,first2);
		}

	//mismatch
	//找出第一个不匹配点
	template<class InputIterator1,class InputIterator2,class Op>
		pair<InputIterator1,InputIterator2> mismatch(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,const Op& op) noexcept
		{
			while(first1 != last1 && op(*first1,*first2))
			{
				++first1;++first2;
			}
			return pair<InputIterator1,InputIterator2>(first1,first2);
		}

	//random_shuffle
	//对区间内的元素随机重新排序
	template<class RandomAccessIterator>
		inline void random_shuffle(RandomAccessIterator first,RandomAccessIterator last) noexcept
		{
			if(first == last)
			{
				return;
			}
			using _dtype = typename iterator_traits<RandomAccessIterator>::distance_type;
			using _itertype = typename iterator_traits<RandomAccessIterator>::iterator_category;
			static_assert(is_random_access_iterator_v<_itertype>);
			for(auto i = first;i != last;++i)
			{
				_iterator_value_swap(i,first + _dtype(rand() % ((i - first) + 1)));
			}
		}


};
#endif
