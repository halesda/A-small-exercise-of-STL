#ifndef _SET_H_
#define _SET_H_
#include"rb_tree.h"
namespace my_stl
{
	//简易set容器实现，底层容器为红黑树，具有自动排序的功能
	//插入，查找，删除时间复杂度都是O(logn)
	template<class Key,class Comp = less<Key>,class Container = rb_tree<Key,Key,Comp>>
		class set
		{
			public:

				using iterator = typename Container::iterator;
				using key_type = Key;
				using value_type = typename Container::value_type;
				using reference = typename Container::reference;
				using pointer = typename Container::pointer;
				using size_type = size_t;
				using distance_type = ptrdiff_t;


				set() noexcept : _con() {}

				~set() noexcept {}

				set(const set& rhs) noexcept : _con(rhs._con) {}

				set& operator= (const set& rhs) noexcept
				{
					if(this != &rhs)
					{
						_con = rhs._con;
					}
					return *this;
				}

				iterator begin()const noexcept
				{
					return _con.begin();
				}

				iterator end() const noexcept
				{
					return _con.end();
				}

				bool empty() const noexcept
				{
					return _con.empty();
				}

				size_t size() const noexcept
				{
					return _con.size();
				}

				iterator insert(const Key& key) noexcept
				{
					return _con.insert(key,key);
				}

				iterator insert(const reference pair) noexcept
				{
					return _con.insert(pair.first,pair.first);
				}

				void erase(iterator pos) noexcept
				{
					return _con.erase(pos);
				}

				void clear() noexcept
				{
					_con.clear();
				}

				iterator find(const Key& key) noexcept
				{
					return _con.find(key);
				}

				bool operator==(const set& rhs) const noexcept
				{
					return _con == rhs._con;
				}

				bool operator!=(const set& rhs) const noexcept
				{
					return _con != rhs._con;
				}

				Key& operator[](const Key& key) noexcept
				{
					iterator i = _con.find(key);
					if(i == end())
					{
						i = _con.insert(key,key);
					}
					return i->second;
				}

				Key& operator[](const Key& key) const noexcept
				{
					iterator i = _con.find(key);
					assert(i != end());
					return i->second;
				}

				void swap(set& rhs) noexcept
				{
					_con.swap(rhs._con);
				}

			private:
				Container _con;
		};

};
#endif
