#ifndef _UNORDERED_MAP_H
#define _UNORDERED_MAP_H
#include"memory.h"
#include"algorithm.h"
#include"type_traits.h"
#include"iterator_traits.h"
#include<cstdint>
#include"vector.h"
#include"string.h"
namespace my_stl
{

	//哈希表使用的类的声明
	template<class Key, class Value,class Alloc>
		class unordered_map;

	template<class Key, class Value>
		struct _hash_node_;

	template<class Key, class Value>
		class _unordered_map_iterator_;

	//下面是哈希函数
	//(long)版本
	static inline size_t BKDHash(const long key) noexcept
	{
		return static_cast<size_t>(key);
	}

	//unsigned long
	static inline size_t BKDHash(const unsigned long key) noexcept
	{
		return static_cast<size_t>(key);
	}

	//int
	static inline size_t BKDHash(const int key) noexcept
	{
		return static_cast<size_t>(key);
	}

	//unsigned int
	static inline size_t BKDHash(const unsigned int key) noexcept
	{
		return static_cast<size_t>(key);
	}

	//string版本
	static inline size_t BKDHash(const string& key) noexcept
	{
		size_t seed = 131, hash = 0, siz = key.size();
		for (size_t i = 0; i < siz; ++i)
		{
			hash = hash * seed + key[i];
		}
		return hash;
	}

	//char*版本
	static inline size_t BKDHash(const char* key) noexcept
	{
		size_t seed = 131, hash = 0, siz = strlen(key);
		for (size_t i = 0; i < siz; ++i)
		{
			hash = hash * seed + key[i];
		}
		return hash;
	}

	//char版本
	static inline size_t BKDHash(const char key) noexcept
	{
		return static_cast<size_t>(key);
	}

	//unsigned char版本
	static inline size_t BKDHash(const unsigned char key) noexcept
	{
		return static_cast<size_t>(key);
	}


	//素数表
	static const size_t PRIME_COUNT = 28;
	static const size_t prime_list[PRIME_COUNT] = 
	{ 
		53ul, 97ul, 193ul, 389ul, 769ul,
		1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
		49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
		1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
		50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};

	static inline size_t _GET_NEXT_PRIME_(size_t now) noexcept
	{
		auto ii = lower_bound(prime_list,prime_list + 28,now);
		if(++ii;ii >= prime_list && ii < prime_list + PRIME_COUNT)
		{
			return *ii;
		}
		return now;
	}


	//哈希节点类型
	template<class Key, class Value>
		struct _hash_node_
		{
			pair<Key, Value>  _data;
			_hash_node_<Key, Value>* _next;
		};


	//哈希迭代器类型
	template<class Key, class Value>
		class _unordered_map_iterator_
		{
			public:

				using _pnode = _hash_node_<Key, Value>*;
				using _node = _hash_node_<Key, Value>;
				using key_type = Key;
				using mapped_type = Value;
				using _ptable = unordered_map<Key, Value,allocator<pair<Key,Value>>> const *;
				using value_type = pair<Key,Value>;
				using iterator_category = forward_iterator_tag;
				using distance_type = ptrdiff_t;
				using reference = value_type&;
				using pointer = value_type*;
				using size_type = ptrdiff_t;

				inline _unordered_map_iterator_(const _pnode p = nullptr , const _ptable t = nullptr )noexcept :_p(p), _t(t) {}

				inline _unordered_map_iterator_(const _unordered_map_iterator_& rhs)noexcept :_p(rhs._p), _t(rhs._t) {}

				inline const _unordered_map_iterator_& operator=(const _unordered_map_iterator_& rhs) noexcept
				{
					if (this != &rhs)
					{
						_p = rhs._p;
						_t = rhs._t;
					}
					return *this;
				}

				inline ~_unordered_map_iterator_() noexcept {}

				//对迭代器解引用返回pair类型
				inline pair<Key, Value>& operator*()const noexcept
				{
					assert(_p);
					return _p->_data;
				}

				//对迭代器使用指针操作符返回pair*
				inline pair<Key, Value>* operator->()const noexcept
				{
					assert(_p);
					return &(_p->_data);
				}

				_unordered_map_iterator_& operator++()noexcept
				{
					if (!_p || !_t)
					{
						return *this;
					}
					if (_p->_next)
					{
						_p = _p->_next;
						return *this;
					}
					else
					{
						size_t siz = _t->_table.size();
						size_t index = (BKDHash(_p->_data.first) % siz) + 1;
						for(;index < siz;++index)
						{
							if(_t->_table[index])
							{
								_p = _t->_table[index];
								return *this;
							}
						}
						_p = nullptr;
						return *this;
					}	
				}

				inline const _unordered_map_iterator_ operator++(int)noexcept
				{
					_unordered_map_iterator_ tmp(*this);
					++(*this);
					return tmp;
				}

				inline bool operator==(const _unordered_map_iterator_& rhs) const noexcept
				{
					return _p == rhs._p && _t == rhs._t;
				}

				inline bool operator!=(const _unordered_map_iterator_& rhs) const noexcept
				{
					return !(*this == rhs);
				}

			private:
				_pnode _p;
				_ptable _t;
		};


	//哈希表
	template<class Key, class Value, class Alloc = allocator<pair<Key,Value>>>
		class unordered_map
		{
			public:

				friend class _unordered_map_iterator_<Key,Value>;
				using _pnode = _hash_node_<Key, Value>*;
				using iterator = _unordered_map_iterator_<Key,Value>;
				using key_type = Key;
				using mapped_type = Value;
				using value_type = pair<Key,Value>;
				using size_type = ptrdiff_t;
				using distance_type = ptrdiff_t;
				using reference = value_type&;
				using pointer = value_type*;

				static_assert(is_same_v<value_type,typename Alloc::value_type>);
				static_assert(is_same_v<value_type,typename iterator::value_type>);
				static_assert(!is_const_v<Value>);
				static_assert(!is_void_v<Value>);
				static_assert(!is_same_v<Key,void>);
				static_assert(!is_same_v<Key,const void>);
				static_assert(!is_same_v<Value,void>);
				static_assert(!is_same_v<Value,const void>);

				inline unordered_map()noexcept :_n(0),_table() {}

				inline ~unordered_map()noexcept
				{
					clear();
				}

				//清空整个哈希表
				void clear()noexcept
				{
					size_t siz = _table.size();
					_pnode pt = nullptr, prev = nullptr;
					for (size_t i = 0; i < siz; ++i)
					{
						pt = _table[i];
						for (; pt; )
						{
							prev = pt;
							pt = pt->_next;
							_destroy_(prev);
						}
						_table[i] = nullptr;
					}
					_table.clear();
					_n = 0;
				}

				//插入一个哈希节点,不允许键值相同
				inline iterator insert(const pair<Key,Value>& pair) noexcept
				{
					return insert(pair.first,pair.second);
				}

				//插入一个哈希节点,不允许键值相同
				iterator insert(const Key& key,const Value& val) noexcept
				{
					iterator i = find(key);
					if (i != end())
					{
						allo.destroy(&i->second);
						allo.construct(&i->second,val);
						return i;
					}
					if (_table.empty())
					{
						_table.resize(53ul, nullptr);
					}
					if ((static_cast<double>(_n) / static_cast<double>(_table.size())) >= 0.75)
					{
						if(!_enhance_size_())
						{
							return end();
						}
					}
					size_t index = BKDHash(key) % _table.size();
					_pnode new_node = _create_(key,val);
					new_node->_next = _table[index];
					_table[index] = new_node;++_n;
					return iterator(new_node,this);
				}

				//返回哈希桶的数量
				inline size_t bucket_size()const noexcept
				{
					size_t count = 0, siz = _table.size();
					for (size_t i = 0; i < siz; ++i)
					{
						if (_table[i])
						{
							++count;
						}
					}
					return count;
				}

				//移除键值为key的哈希节点
				void remove(const Key& key) noexcept
				{
					if (_n == 0 || _table.size() <= 0)
					{
						return;
					}
					size_t index = BKDHash(key) % _table.size();
					_pnode pt = _table[index],prev = pt;
					if(pt)
					{
						if(pt->_data.first == key)
						{
							_table[index] = pt->_next;
						}
						else
						{
							for(pt = pt->_next;pt && pt->_data.first != key;pt = pt->_next)
							{
								prev = pt;
							}
							prev->_next = pt->_next;
						}
						if(pt)
						{
							--_n;
							_destroy_(pt);
						}
					}
				}

				//移除键值为key的哈希节点
				inline void remove(const pair<Key, Value>& pair) noexcept
				{
					remove(pair.first);
				}

				//返回哈希节点数量
				inline size_t size() const noexcept
				{
					return _n;
				}

				//返回第一个哈希节点迭代器
				inline iterator begin()const noexcept
				{
					size_t siz = _table.size();
					for (size_t i = 0; i < siz; ++i)
					{
						if (_table[i])
						{
							return iterator(_table[i], this);
						}
					}
					return iterator(nullptr, this);
				}

				//返回尾迭代器
				inline iterator end() const noexcept
				{
					return iterator(nullptr, this);
				}

				//查找键值为key的元素
				inline iterator find(const pair<Key, Value>& pair)const noexcept
				{
					return find(pair.key);
				}

				//查找键值为key的元素
				iterator find(const Key& key) const noexcept
				{
					if(_n == 0 || _table.size() <= 0)
					{
						return end();
					}
					size_t index = BKDHash(key) % _table.size();
					_pnode pt = _table[index];
					for (; pt ; pt = pt->_next)
					{
						if (pt->_data.first == key)
						{
							return iterator(pt,this);
						}
					}
					return end();
				}

				//重载[]操作符，如果键值key对应的元素不存在则插入一个新元素
				inline Value& operator[](const Key& key) noexcept
				{
					auto iter = find(key);
					if(iter == end())
					{
						iter = insert(key,Value());
					}
					return iter->second;
				}

				//const版本的不允许插入
				inline Value& operator[](const Key& key) const noexcept
				{
					auto iter = find(key);
					assert(iter != end());
					return iter->second;
				}

			protected:

				//摧毁一个哈希节点
				inline void _destroy_(_pnode pt) noexcept
				{
					allo.destroy(&pt->_data.first);
					allo.destroy(&pt->_data.second);
					allo.deallocate(pt);
				}

				//创造一个哈希节点
				inline _pnode _create_(const Key& key,const Value& val) noexcept
				{
					_pnode new_node = static_cast<_pnode>(allo.allocate(sizeof(_hash_node_<Key, Value>)));
					allo.construct(&new_node->_data.first, key);
					allo.construct(&new_node->_data.second, val);
					return new_node;
				}

				//增加哈希表的容量
				bool _enhance_size_() noexcept
				{
					size_t siz = _table.size();
					size_t nxt = _GET_NEXT_PRIME_(siz);
					if(nxt == siz)
					{
						return false;
					}
					vector<_pnode> new_table(nxt, nullptr);
					_pnode pt = nullptr;
					for (size_t i = 0; i < siz; ++i)
					{
						pt = _table[i];
						if (pt)
						{
							new_table[BKDHash(pt->_data.first) % nxt] = pt;
							_table[i] = nullptr;
						}
					}
					_table.swap(new_table);
					return true;
				}

			private:

				vector<_pnode> _table;
				size_t _n;
				Alloc allo;
		};
};
#endif
