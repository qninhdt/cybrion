#ifndef JBT_TAG_H
#define JBT_TAG_H

#include "jbt/internal.hpp"

#define JBT_TAG_METHOD(name_, type)                                \
	tag(const type& value);                                        \
	type& get_##name_(const std::string& name) const;              \
	void set_##name_(const std::string& name, const type& value);  \
	type& get_##name_(const uint32_t& index) const;                \
	void set_##name_(const uint32_t& index, const type& value);    \
	void add_##name_(const type& value);

namespace jbt {

	class tag {

	public:
		tag_type get_type() const;

		tag();
		tag(const tag_type& type);
		tag(tag_type&& type);

		tag(tag&& other) noexcept;
		tag(std::string&& other) noexcept;

		tag& operator=(const tag& other);
		tag& operator=(tag&& other) noexcept;

		~tag();

		void setup_data();

		JBT_TAG_METHOD(bool, bool)
		JBT_TAG_METHOD(byte, int8_t)
		JBT_TAG_METHOD(ubyte, uint8_t)
		JBT_TAG_METHOD(short, int16_t)
		JBT_TAG_METHOD(ushort, uint16_t)
		JBT_TAG_METHOD(int, int32_t)
		JBT_TAG_METHOD(uint, uint32_t)
		JBT_TAG_METHOD(long, int64_t)
		JBT_TAG_METHOD(ulong, uint64_t)
		JBT_TAG_METHOD(float, float)
		JBT_TAG_METHOD(double, double)
		JBT_TAG_METHOD(string, std::string)
		JBT_TAG_METHOD(tag, tag)
		JBT_TAG_METHOD(byte_array, byte_array_t)

		void set_string(const std::string& index, std::string&& value);
		void set_string(const uint32_t& index, std::string&& value);
		void add_string(std::string&& value);

		void set_tag(const std::string& index, tag&& value);
		void set_tag(const uint32_t& index, tag&& value);
		void add_tag(tag&& value);

		uint32_t size() const;
		void reserve(const uint32_t& size);
		void resize(const uint32_t& size);

		bool& as_bool();
		int8_t& as_byte();
		uint8_t& as_ubyte();
		int16_t& as_short();
		uint16_t& as_ushort();
		int32_t& as_int();
		uint32_t& as_uint();
		int64_t& as_long();
		uint64_t& as_ulong();
		float& as_float();
		double& as_double();
		std::string& as_string();
		byte_array_t& as_byte_array();

		void to_string(std::ostream& out) const;
		
	private:

		friend class serializer;
		
		void to_string(std::ostream& out, const int32_t& tabs) const;
		
		union data_t {
			bool          v_bool;
			int8_t        v_byte;
			uint8_t       v_ubyte;
			int16_t       v_short;
			uint16_t      v_ushort;
			int32_t       v_int;
			uint32_t      v_uint;
			int64_t       v_long;
			uint64_t      v_ulong;
			float         v_float;
			double        v_double;
			object_t*     v_object;
			list_t*       v_list;
			std::string*  v_string;
			byte_array_t* v_byte_array;
		} data;

		tag_type type;
	};

	std::ostream& operator<<(std::ostream& os, const tag_type& type);
	std::ostream& operator<<(std::ostream& os, const tag& a_tag);
}

#endif // !JBT_TAG_H
