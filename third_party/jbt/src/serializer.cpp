#include "jbt/serializer.hpp"
#include "jbt/tag.hpp"
#include <iostream>

namespace jbt {

	serializer* serializer::instance;

	void serializer_util::swap_2_bytes(char* number) {
		std::swap(number[0], number[1]);
	}

	void serializer_util::swap_4_bytes(char* number) {
		std::swap(number[0], number[3]);
		std::swap(number[1], number[2]);
	}

	void serializer_util::swap_8_bytes(char* number) {
		std::swap(number[0], number[7]);
		std::swap(number[1], number[6]);
		std::swap(number[2], number[5]);
		std::swap(number[3], number[4]);
	}

	void serializer::read_bool(std::istream& input, bool& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(bool));
	}

	void serializer::read_string(std::istream& input, std::string& result) {
		uint16_t size;

		// read size
		read_ushort(input, size);
		result.resize(static_cast<size_t>(size));

		// read data
		input.read(result.data(), size * sizeof(char));
	}

	void serializer::read_byte(std::istream& input, int8_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int8_t));
	}
	void serializer::read_ubyte(std::istream& input, uint8_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint8_t));
	}
	void serializer::read_float(std::istream& input, float& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(float));
	}
	void serializer::read_double(std::istream& input, double& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(double));
	}

	void serializer::read_tag_type(std::istream& input, tag_type& result) {
		uint8_t id;
		read_ubyte(input, id);
		result = static_cast<tag_type>(id);
	}

	void serializer::read_object(std::istream& input, object_t& result) {
		uint16_t size;
		read_ushort(input, size);

		for (int i = 0; i < size; ++i) {
			// read name
			uint8_t name_size;
			read_ubyte(input, name_size);

			std::string name;
			name.resize(name_size);

			input.read(name.data(), name_size);

			// read tag payload
			tag a_tag;
			read_tag(input, a_tag);

			result[name] = std::move(a_tag);
		}
	}

	void serializer::read_list(std::istream& input, list_t& result) {
		uint32_t size;
		read_uint(input, size);

		result.resize(size);

		for (int i = 0; i < size; ++i) {
			read_tag(input, result[i]);
		}
	}

	void serializer::read_tag(std::istream& input, tag& result) {
		read_tag_type(input, result.type);
		result.setup_data();

		switch (result.type) {
		case jbt::tag_type::NONE:
			assert(false && "None type tag is not allowed");
			break;
		case jbt::tag_type::BOOL:
			read_bool(input, result.data.v_bool);
			break;
		case jbt::tag_type::LIST:
			read_list(input, *result.data.v_list);
			break;
		case jbt::tag_type::STRING:
			read_string(input, *result.data.v_string);
			break;
		case jbt::tag_type::OBJECT:
			read_object(input, *result.data.v_object);
			break;
		case jbt::tag_type::BYTE:
			read_byte(input, result.data.v_byte);
			break;
		case jbt::tag_type::UBYTE:
			read_ubyte(input, result.data.v_ubyte);
			break;
		case jbt::tag_type::SHORT:
			read_short(input, result.data.v_short);
			break;
		case jbt::tag_type::USHORT:
			read_ushort(input, result.data.v_ushort);
			break;
		case jbt::tag_type::INT:
			read_int(input, result.data.v_int);
			break;
		case jbt::tag_type::UINT:
			read_uint(input, result.data.v_uint);
			break;
		case jbt::tag_type::LONG:
			read_long(input, result.data.v_long);
			break;
		case jbt::tag_type::ULONG:
			read_ulong(input, result.data.v_ulong);
			break;
		case jbt::tag_type::FLOAT:
			read_float(input, result.data.v_float);
			break;
		case jbt::tag_type::DOUBLE:
			read_double(input, result.data.v_double);
			break;
		case jbt::tag_type::BYTE_ARRAY:
			read_byte_array(input, *result.data.v_byte_array);
			break;
		default:
			std::cout << int(result.type) << '\n';
			assert(false && "Unknown tag type");
			break;
		}
	}

	void serializer::read_byte_array(std::istream& input, byte_array_t& result) {
		read_uint(input, result.size);
		result.data = new int8_t[result.size];
		result.is_owner = true;
		input.read(reinterpret_cast<char*>(result.data), result.size);
	}

	void serializer::write_bool(std::ostream& output, const bool& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(bool));
	}

	void serializer::write_string(std::ostream& output, const std::string& value) {
		size_t size = value.size();
		
		// write string size
		write_ushort(output, static_cast<uint16_t>(size));

		// write string data
		output.write(value.c_str(), size * sizeof(char));
	}

	void serializer::write_byte(std::ostream& output, const int8_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(int8_t));
	}

	void serializer::write_ubyte(std::ostream& output, const uint8_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(uint8_t));
	}

	void serializer::write_float(std::ostream& output, const float& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(float));
	}

	void serializer::write_double(std::ostream& output, const double& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(double));
	}

	void serializer::write_tag_type(std::ostream& output, const tag_type& value) {
		write_ubyte(output, static_cast<uint8_t>(value));
	}

	void serializer::write_object(std::ostream& output, const object_t& value) {
		const size_t size = value.size();
		assert(size <= 65535 && "Maximum number of data.v_list in an object is 65535");

		write_ushort(output, static_cast<uint16_t>(size));

		for (const auto& [name, a_tag] : value) {
			assert(a_tag.get_type() != tag_type::NONE && "None type tag is not allowed");

			// write name
			uint8_t name_size = static_cast<uint8_t>(name.size());

			write_ubyte(output, name_size);
			output.write(name.c_str(), name_size);

			// write tag
			write_tag(output, a_tag);
		}
	}

	void serializer::write_list(std::ostream& output, const list_t& value) {
		write_uint(output, value.size());

		for (const auto& a_tag : value) {
			assert(a_tag.get_type() != tag_type::NONE && "None type tag is not allowed");
			write_tag(output, a_tag);
		}
	}

	void serializer::write_tag(std::ostream& output, const tag& value) {
		write_tag_type(output, value.type);

		switch (value.type) {
		case jbt::tag_type::NONE:
			assert(false && "None type tag is not allowed");
			break;
		case jbt::tag_type::BOOL:
			write_bool(output, value.data.v_bool);
			break;
		case jbt::tag_type::LIST:
			write_list(output, *value.data.v_list);
			break;
		case jbt::tag_type::STRING:
			write_string(output, *value.data.v_string);
			break;
		case jbt::tag_type::OBJECT:
			write_object(output, *value.data.v_object);
			break;
		case jbt::tag_type::BYTE:
			write_byte(output, value.data.v_byte);
			break;
		case jbt::tag_type::UBYTE:
			write_ubyte(output, value.data.v_ubyte);
			break;
		case jbt::tag_type::SHORT:
			write_short(output, value.data.v_short);
			break;
		case jbt::tag_type::USHORT:
			write_ushort(output, value.data.v_ushort);
			break;
		case jbt::tag_type::INT:
			write_int(output, value.data.v_int);
			break;
		case jbt::tag_type::UINT:
			write_uint(output, value.data.v_uint);
			break;
		case jbt::tag_type::LONG:
			write_long(output, value.data.v_long);
			break;
		case jbt::tag_type::ULONG:
			write_ulong(output, value.data.v_ulong);
			break;
		case jbt::tag_type::FLOAT:
			write_float(output, value.data.v_float);
			break;
		case jbt::tag_type::DOUBLE:
			write_double(output, value.data.v_double);
			break;
		case tag_type::BYTE_ARRAY:
			write_byte_array(output, *value.data.v_byte_array);
			break;
		default:
			assert(false && "Unknown tag type");
			break;
		}
	}

	void serializer::write_byte_array(std::ostream& output, const byte_array_t& value) {
		write_uint(output, value.size);
		output.write(reinterpret_cast<char*>(value.data), value.size);
	}

	void diff_endian_serializer::read_short(std::istream& input, int16_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int16_t));
		serializer_util::swap_2_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::read_ushort(std::istream& input, uint16_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint16_t));
		serializer_util::swap_2_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::read_int(std::istream& input, int32_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int32_t));
		serializer_util::swap_4_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::read_uint(std::istream& input, uint32_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
		serializer_util::swap_4_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::read_long(std::istream& input, int64_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int64_t));
		serializer_util::swap_8_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::read_ulong(std::istream& input, uint64_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint64_t));
		serializer_util::swap_8_bytes(reinterpret_cast<char*>(&result));
	}
	void diff_endian_serializer::write_short(std::ostream& output, const int16_t& value) {
		int16_t xvalue = value;
		serializer_util::swap_2_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(int16_t));
	}

	void diff_endian_serializer::write_ushort(std::ostream& output, const uint16_t& value) {
		int16_t xvalue = value;
		serializer_util::swap_2_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(uint16_t));
	}

	void diff_endian_serializer::write_int(std::ostream& output, const int32_t& value) {
		int32_t xvalue = value;
		serializer_util::swap_4_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(int32_t));
	}

	void diff_endian_serializer::write_uint(std::ostream& output, const uint32_t& value) {
		uint32_t xvalue = value;
		serializer_util::swap_4_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(uint32_t));
	}

	void diff_endian_serializer::write_long(std::ostream& output, const int64_t& value) {
		int64_t xvalue = value;
		serializer_util::swap_8_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(int64_t));
	}

	void diff_endian_serializer::write_ulong(std::ostream& output, const uint64_t& value) {
		uint64_t xvalue = value;
		serializer_util::swap_8_bytes(reinterpret_cast<char*>(&xvalue));
		output.write(reinterpret_cast<const char*>(&xvalue), sizeof(uint64_t));
	}

	void same_endian_serializer::read_short(std::istream& input, int16_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int16_t));
	}
	void same_endian_serializer::read_ushort(std::istream& input, uint16_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint16_t));
	}
	void same_endian_serializer::read_int(std::istream& input, int32_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int32_t));
	}
	void same_endian_serializer::read_uint(std::istream& input, uint32_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
	}
	void same_endian_serializer::read_long(std::istream& input, int64_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(int64_t));
	}
	void same_endian_serializer::read_ulong(std::istream& input, uint64_t& result) {
		input.read(reinterpret_cast<char*>(&result), sizeof(uint64_t));
	}
	void same_endian_serializer::write_short(std::ostream& output, const int16_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(int16_t));
	}

	void same_endian_serializer::write_ushort(std::ostream& output, const uint16_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(uint16_t));
	}

	void same_endian_serializer::write_int(std::ostream& output, const int32_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
	}

	void same_endian_serializer::write_uint(std::ostream& output, const uint32_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
	}

	void same_endian_serializer::write_long(std::ostream& output, const int64_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(int64_t));
	}

	void same_endian_serializer::write_ulong(std::ostream& output, const uint64_t& value) {
		output.write(reinterpret_cast<const char*>(&value), sizeof(uint64_t));
	}
}

