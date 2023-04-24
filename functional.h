#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_
namespace my_stl
{

	//unary_function用来呈现一元函数的参数型别和返回值型别
	template<class Arg,class Result>
		struct unary_function
		{
			using argument_type = Arg;
			using result_type = Result;
		};

	//binary_function用来展现二元函数的第一参数型别和第二参数型别，和返回值型别
	template<class Arg1,class Arg2,class Result>
		struct binary_function
		{
			using first_argument_type = Arg1;
			using second_argument_type = Arg2;
			using result_type = Result;
		};


	//算数类型仿函数
	template<class T>
		struct plus : public binary_function<T,T,T>
	{
		inline T operator()(const T& lhs,const T& rhs) const
		{
			return lhs + rhs;
		}
	};

	template<class T>
		struct negate : public unary_function<T,T>
	{
		inline T operator()(const T& val) const
		{
			return -val;
		}
	};

	template<class T>
		struct minus : public unary_function<T,T>
	{
		inline T operator()(const T& lhs,const T& rhs) const
		{
			return lhs - rhs;
		}
	};

	template<class T>
		struct divides : public unary_function<T,T>
	{
		inline T operator()(const T& lhs,const T& rhs) const
		{
			return lhs / rhs;
		}
	};

	template<class T>
		struct multiplies : public unary_function<T,T>
	{
		inline T operator()(const T& lhs,const T& rhs) const
		{
			return lhs * rhs;
		}
	};

	template<class T>
		struct modulus : public unary_function<T,T>
	{
		inline T operator()(const T& lhs,const T& rhs) const
		{
			return lhs % rhs;
		}
	};


	//关系运算仿函数
	template<class T>
		struct less : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs < rhs;
		}
	};

	template<class T>
		struct less_equal : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs <= rhs;
		}
	};

	template<class T>
		struct equal_to : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs == rhs;
		}
	};

	template<class T>
		struct not_equal_to : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs != rhs;
		}
	};

	template<class T>
		struct greater : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs > rhs;
		}
	};

	template<class T>
		struct greater_equal : public binary_function<T,T,bool>
	{
		inline bool operator()(const T& lhs,const T& rhs) const
		{
			return lhs >= rhs;
		}
	};

};
#endif
