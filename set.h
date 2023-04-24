#ifndef _SET_H_
#define _SET_H_
namespace my_stl
{
#include"rb_tree.h"

	template<class Key,class Comp = less<Key>,class Container = rb_tree<Key,Key,Comp>>
		class set
		{
			public:

				using iterator = typename Container::iterator;
				using key_type = Key;
				using value_type = typename Container::value_type;
				using reference = value_type&;
				using pointer = value_type*;
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

				iterator insert(const value_type& pair) noexcept
				{
					return _con.insert(pair);
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

				bool operator==(const set& rhs) noexcept
				{
					return _con == rhs._con;
				}

				bool operator!=(const set& rhs) noexcept
				{
					return _con != rhs._con;
				}

				Key& operator[](const Key& key) noexcept
				{
					iterator i = find(key);
					if(i == end())
					{
						i = _con.insert(key,Key());
					}
					return i->second;
				}

				Key& operator[](const Key& key) const noexcept
				{
					iterator i = find(key);
					assert(i != end());
					return i->second;
				}

			private:
				Container _con;
		};

};
#endif
