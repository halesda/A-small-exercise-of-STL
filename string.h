#ifndef _STRING_H_
#define _STRING_H_
#include"allocator.h"
#include"memory.h"
#include"algorithm.h"
namespace my_stl
{

	static inline size_t strlen(const char* str) noexcept
	{
		size_t n = 0;
		for(;str && *str != '\0';++str)
		{
			++n;
		}
		return n;
	}

	class string
	{
	public:

		using iterator = char*;
		using distance_type = ptrdiff_t;
		using value_type = char;
		using size_type = ptrdiff_t;


		string(const char* str = nullptr) noexcept
		{
			size_t n = strlen(str);
			if(n == 0)
			{
				_first = static_cast<char*>(allo.allocate(10));
				*_first = '\0';
				_last = _first + 1;
				_end = _first + 10;
			}
			else
			{
				_first = static_cast<char*>(allo.allocate(n * 2));
				copy(str,str + n,_first);
				_first[n] = '\0';
				_last = _first + n + 1;
				_end = _first + (2 * n);
			}
		}

		template<class Size>
		inline string(Size n,char c) noexcept
		{
			if(n <= 0)
			{
				_first = static_cast<char*>(allo.allocate(10));
				*_first = '\0';
				_last = _first + 1;
				_end = _first + 10;
				return;
			}
			_first = static_cast<char*>(allo.allocate(n * 2));
			uninitialized_fill_n(_first,n,c);
			_first[n] = '\0';
			_last = _first + n + 1;
			_end = _first + (2 * n);
		}

		string(const string& str) noexcept
		{
			size_t n = strlen(str._first);
			if(n == 0)
			{
				_first = static_cast<char*>(allo.allocate(10));
				*_first = '\0';
				_last = _first + 1;
				_end = _first + 10;
			}
			else
			{
				_first = static_cast<char*>(allo.allocate(n * 2));
				copy(str._first,str._first + n,_first);
				_first[n] = '\0';
				_last = _first + n + 1;
				_end = _first + (2 * n);
			}
		}

		void pop_back() noexcept
		{
			if(_first == nullptr || _first == _last)
			{
				return;
			}
			--_last;
			*(_last - 1) = '\0';
		}

		~string() noexcept
		{
			clear();
		}

		string& operator= (const string& rhs) noexcept
		{
			if(this != &rhs)
			{
				clear();
				push_back(rhs._first);
			}
			return *this;
		}

		void clear() noexcept
		{
			if(_first)
			{
				allo.deallocate(_first);
			}
			_first = _last = _end = nullptr;
		}

		void push_back(const char* str) noexcept
		{
			size_t n = strlen(str);
			if(!str || n == 0)
			{
				return;
			}
			if(_first == nullptr)
			{
				_first = static_cast<char*>(allo.allocate(n * 2));
				copy(str,str + n,_first);
				_first[n] = '\0';
				_last = _first + n + 1;
				_end = _first + n * 2;
			}
			else if(n <= _end - _last)
			{
				copy(str,str + n,_last - 1);
				_last += n;
				*(_last - 1) = '\0';
			}
			else
			{
				size_t siz = strlen(_first);
				auto ffirst = _first;
				_first = static_cast<char*>(allo.allocate(siz * 2 + n));
				copy(ffirst,ffirst + siz,_first);
				copy(str,str + n,_first + siz);
				_last = _first + siz + n + 1;
				*(_last - 1) = '\0';
				_end = _first + siz * 2 + n;
				allo.deallocate(ffirst);
			}
		}

		void push_back(const char c) noexcept
		{
			if(_first  == nullptr)
			{
				_first = static_cast<char*>(allo.allocate(10));
				*_first = c;
				_end = _first + 10;
				_last = _first + 2;
				*(_last - 1) = '\0';
			}
			else if(_last + 1 <= _end)
			{
				*(_last - 1) = c;
				*_last = '\0';
				++_last;
			}
			else
			{
				size_t siz = strlen(_first);
				auto ffirst = _first;
				_first = static_cast<char*>(allo.allocate(2 * siz + 1));
				_end = _first + 2 * siz + 1;
				copy(ffirst,ffirst + siz,_first);
				_last = _first + siz;
				*_last = c;
				*(++_last) = '\0';
				++_last;
				allo.deallocate(ffirst);
			}
		}

		void push_back(const string& str) noexcept
		{
			push_back(str._first);
		}

		friend std::ostream& operator <<(std::ostream& os,const string& str) noexcept
		{
			if(str._first)
			{
				for(char* i = str._first;*i != '\0';++i)
				{
					os << *i;
				}
			}
			return os;
		}

		string& operator +=(const string& str) noexcept
		{
			push_back(str);
			return *this;
		}

		const string operator +(const string& str) noexcept
		{
			return string(*this) += str;
		}

		string& operator +=(const char* str) noexcept
		{
			push_back(str);
			return *this;
		}

		const string operator +(const char* str) noexcept
		{
			return string(*this) += str;
		}

		string& operator +=(const char str) noexcept
		{
			push_back(str);
			return *this;
		}

		const string operator +(const char str) noexcept
		{
			return string(*this) += str;
		}

		friend std::istream& operator >>(std::istream& is,string& str) noexcept
		{
			if(str._first)
			{
				str.clear();
			}
			char tmp[1024];
			is >> tmp;
			size_t n = strlen(tmp);
			str._first = static_cast<char*>(str.allo.allocate(n * 2));
			copy(tmp,tmp + n,str._first);
			str._first[n] = '\0';
			str._last = str._first + n + 1;
			str._end = str._first + n * 2;
			return is;
		}

		inline const char* c_str() const
		{
			return _first;
		}

		inline size_t size() const
		{
			return strlen(_first);
		}

		inline size_t capacity() const
		{
			return _end - _first;
		}

		void erase(iterator pos) noexcept
		{
			if(!pos || !_first)
			{
				return;
			}
			copy(pos + 1,_last,pos);
			--_last;
			*(_last - 1) = '\0';
		}

		iterator begin() const
		{
			return _first;
		}

		iterator end() const
		{
			return _last - 1;
		}

		bool operator ==(const string& str) const noexcept
		{
			size_t s1 = size(),s2 = str.size();
			if(s1 != s2)
			{
				return false;
			}
			for(size_t i = 0;i < s1;++i)
			{
				if(_first[i] != str._first[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator !=(const string& str) const noexcept
		{
			return !(*this == str);
		}

		char& operator[](size_t n) noexcept
		{
			assert(_first && n < size());
			return *(_first + n);
		}

		char& operator[](size_t n)const noexcept
		{
			assert(_first && n < size());
			return *(_first + n);
		}

	private:
		allocator<char> allo;
		char* _first;
		char* _last;
		char* _end;
	};
};
#endif
