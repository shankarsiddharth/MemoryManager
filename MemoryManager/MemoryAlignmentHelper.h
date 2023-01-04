#pragma once

#include <cstdint>
#include <cassert>

class MemoryAlignmentHelper
{
public:
	static inline size_t AlignSize(size_t i_size, size_t i_align = sDefaultAlignment) {
		return (i_size + i_align - 1) & ~(i_align - 1);
	}

	static inline size_t AlignPadding(size_t i_size, size_t i_align = sDefaultAlignment) {
		return AlignSize(i_size, i_align) - i_size;
	}

	static inline bool IsPowerOfTwo(size_t i_align)
	{
		return (i_align & (i_align - 1)) == 0;
	}

	static inline uintptr_t AlignDown(uintptr_t i_value, size_t i_align = sDefaultAlignment)
	{
		assert(i_align);
		assert(IsPowerOfTwo(i_align));
		return i_value & ~(i_align - 1);
	}

	static inline uintptr_t AlignUp(uintptr_t i_value, size_t i_align = sDefaultAlignment)
	{
		assert(i_align);
		assert(IsPowerOfTwo(i_align));
		return (i_value + (i_align - 1)) & ~(i_align - 1);
	}

	static const size_t sDefaultAlignment = sizeof(size_t);
};
