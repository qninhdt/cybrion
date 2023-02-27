#ifndef JBT_SERIALIZER_H
#define JBT_SERIALIZER_H

#include "jbt/internal.hpp"

namespace jbt {

	class serializer_util {
	public:
		static void swap_2_bytes(char* number);
		static void swap_4_bytes(char* number);
		static void swap_8_bytes(char* number);
	};

	class serializer {

	public:
		void read_bool(std::istream& input, bool& result);
		void read_string(std::istream& input, std::string& result);
		void read_byte(std::istream& input, int8_t& result);
		void read_ubyte(std::istream& input, uint8_t& result);
		virtual void read_short(std::istream& input, int16_t& result) = 0;
		virtual void read_ushort(std::istream& input, uint16_t& result) = 0;
		virtual void read_int(std::istream& input, int32_t& result) = 0;
		virtual void read_uint(std::istream& input, uint32_t& result) = 0;
		virtual void read_long(std::istream& input, int64_t& result) = 0;
		virtual void read_ulong(std::istream& input, uint64_t& result) = 0;
		void read_float(std::istream& input, float& result);
		void read_double(std::istream& input, double& result);
		void read_object(std::istream& input, object_t& result);
		void read_list(std::istream& input, list_t& result);
		void read_tag(std::istream& input, tag& result);
		void read_tag_type(std::istream& input, tag_type& result);
		void read_byte_array(std::istream& input, byte_array_t& result);

		void write_bool(std::ostream& output, const bool& value);
		void write_string(std::ostream& output, const std::string& value);
		void write_byte(std::ostream& output, const int8_t& value);
		void write_ubyte(std::ostream& output, const uint8_t& value);
		virtual void write_short(std::ostream& output, const int16_t& value) = 0;
		virtual void write_ushort(std::ostream& output, const uint16_t& value) = 0;
		virtual void write_int(std::ostream& output, const int32_t& value) = 0;
		virtual void write_uint(std::ostream& output, const uint32_t& value) = 0;
		virtual void write_long(std::ostream& output, const int64_t& value) = 0;
		virtual void write_ulong(std::ostream& output, const uint64_t& value) = 0;
		void write_float(std::ostream& output, const float& value);
		void write_double(std::ostream& output, const double& value);
		void write_object(std::ostream& output, const object_t& value);
		void write_list(std::ostream& output, const list_t& value);
		void write_tag(std::ostream& output, const tag& value);
		void write_tag_type(std::ostream& output, const tag_type& value);
		void write_byte_array(std::ostream& output, const byte_array_t& value);
		void write_and_delete_tag(); /// TODO: implement later

		static serializer* instance;
	};

	class diff_endian_serializer : public serializer {
		void read_short(std::istream& input, int16_t& result);
		void read_ushort(std::istream& input, uint16_t& result);
		void read_int(std::istream& input, int32_t& result);
		void read_uint(std::istream& input, uint32_t& result);
		void read_long(std::istream& input, int64_t& result);
		void read_ulong(std::istream& input, uint64_t& result);
		void write_short(std::ostream& output, const int16_t& value);
		void write_ushort(std::ostream& output, const uint16_t& value);
		void write_int(std::ostream& output, const int32_t& value);
		void write_uint(std::ostream& output, const uint32_t& value);
		void write_long(std::ostream& output, const int64_t& value);
		void write_ulong(std::ostream& output, const uint64_t& value);
	};

	class same_endian_serializer : public serializer {
		void read_short(std::istream& input, int16_t& result);
		void read_ushort(std::istream& input, uint16_t& result);
		void read_int(std::istream& input, int32_t& result);
		void read_uint(std::istream& input, uint32_t& result);
		void read_long(std::istream& input, int64_t& result);
		void read_ulong(std::istream& input, uint64_t& result);
		void write_short(std::ostream& output, const int16_t& value);
		void write_ushort(std::ostream& output, const uint16_t& value);
		void write_int(std::ostream& output, const int32_t& value);
		void write_uint(std::ostream& output, const uint32_t& value);
		void write_long(std::ostream& output, const int64_t& value);
		void write_ulong(std::ostream& output, const uint64_t& value);
	};
}

#endif // !JBT_SERIALIZER_H