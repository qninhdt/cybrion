#ifndef JBT_INTERNAL
#define JBT_INTERNAL

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

namespace jbt {

	class tag;

	using object_t = std::map<std::string, tag>;
	using list_t = std::vector<tag>;

	struct byte_array_t {
		int8_t* data;
		uint32_t size;
		bool is_owner;
	};

	enum class tag_type : uint8_t {
		NONE,
		BOOL,
		LIST,
		STRING,
		OBJECT,
		BYTE_ARRAY,
		BYTE,
		UBYTE,
		SHORT,
		USHORT,
		INT,
		UINT,
		LONG,
		ULONG,
		FLOAT,
		DOUBLE
	};
}

#endif // !JBT_INTERNAL
