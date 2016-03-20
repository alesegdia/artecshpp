#pragma once

#include <vector>
#include <cstdio>

struct BasePool {
	BasePool( size_t itemSize, size_t numItems )
		: m_itemSize(itemSize), m_numItems(numItems) { expand(numItems); }
	virtual ~BasePool() {
		for( auto block : m_blocks )
		{
			delete [] block;
		}
	}

	inline void expand(size_t n)
	{
		n = n+1;
		if( m_size < n )
		{
			if( m_capacity < n )
			{
				reserve(n);
			}
			m_size = n;
		}
	}

	inline void reserve(size_t n)
	{
		while( m_capacity < n )
		{
			char* block = new char[m_itemSize * m_numItems];
			m_blocks.push_back(block);
			m_capacity += m_numItems;
		}
	}

	inline void* get( size_t n )
	{
		size_t block_index = n / m_numItems;
		size_t block_offset = ( n % m_numItems ) * m_itemSize;
		return m_blocks[block_index] + block_offset;
	}

	virtual void destroy( size_t n ) = 0;

	std::vector<char*> m_blocks; 	// data blocks
	size_t m_itemSize; 				// size of an item
	size_t m_numItems; 				// number of items per block
	size_t m_size = 0; 				// current pool size in number of items
	size_t m_capacity; 				// room in number of items to store without realloc
};

template <typename T, size_t ItemsPerBlock = 4096>
struct Pool : public BasePool {
	Pool() : BasePool(sizeof(T), ItemsPerBlock) {}
	virtual ~Pool() {}

	virtual void destroy( size_t n )
	{
		T* element = static_cast<T*>(get(n));
		element->~T();
	}
};

