#ifndef _MAP_H_
#define _MAP_H_
#include"rb_tree.h"
namespace my_stl
{

	//简易map容器实现，底层容器为红黑树，具有自动排序的功能
	//插入，查找，删除时间复杂度都是O(logn)
	template<class Key,class Value,class Comp = less<Key>,class Container = rb_tree<Key,Value,Comp>>
		class map
		{
			public:

				using value_type = pair<Key,Value>;
				using key_type = Key;
				using mapped_type = Value;
				using size_type = size_t;
				using distance_type = ptrdiff_t;
				using iterator = typename Container::iterator;

				map() noexcept : _con() {}

				map(const map& rhs) noexcept : _con(rhs._con) {}

				map& operator= (const map& rhs) noexcept
				{
					if(this != rhs)
					{
						_con = rhs._con;
					}
					return *this;
				}

				~map() noexcept
				{
					_con.clear();
				}

				void insert(const Key& key,const Value& val) noexcept
				{
					_con.insert(key,val);
				}

				void insert(const value_type& pir) noexcept
				{
					_con.insert(pir.first,pir.second);
				}

				iterator begin() const noexcept
				{
					return _con.begin();
				}

				iterator end() const noexcept
				{
					return _con.end();
				}

				size_t size() const noexcept
				{
					return _con.size();
				}

				iterator find(const Key& key) const noexcept
				{
					return _con.find(key);
				}

				Value& operator[](const Key& key) noexcept
				{
					iterator i = find(key);
					if(i == end())
					{
						i = insert(key,Value());
					}
					return i->second;
				}

				Value& operator[](const Key& key) const noexcept
				{
					iterator i = find(key);
					assert(i != end());
					return i->second;
				}

				bool empty() const noexcept
				{
					return _con.empty();
				}

				void erase(const Key& key) noexcept
				{
					_con.erase(key);
				}

			private:
				Container _con;
		};
};
#endif
