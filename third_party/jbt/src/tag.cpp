#include "jbt/tag.hpp"
#include "jbt/serializer.hpp"
#include <iomanip>

#define TYPE_CHECK(tag, correct_type) \
	assert(tag##.type == tag_type::##correct_type && "Wrong tag type")

#define STRING_CHECK(value) \
	assert(value.size() <= 65535 && "Maximum length of string is 65535")

#define JBT_TAG_METHOD_IMPL(name_, type_, enum_type)                   \
	tag::tag(const type_& value): type(tag_type::##enum_type) {\
		data.v_##name_ = value;                                          \
	}\
	type_& tag::get_##name_(const std::string& name) const {               \
		TYPE_CHECK((*this), OBJECT);\
		tag& a_tag = get_tag(name);                                               \
		return a_tag.as_##name_();                    \
	}                                                                           \
	void tag::set_##name_(const std::string& name, const type_& value) {  \
		TYPE_CHECK((*this), OBJECT);\
		set_tag(name, { value });                                  \
	}                     \
	type_& tag::get_##name_(const uint32_t& index) const {                    \
		TYPE_CHECK((*this), LIST);\
		tag& a_tag = get_tag(index);                                              \
		return a_tag.as_##name_();                      \
	}                                                                           \
	void tag::set_##name_(const uint32_t& index, const type_& value) {       \
		TYPE_CHECK((*this), LIST);\
		set_tag(index, { value });                                  \
	}                                                                           \
	void tag::add_##name_(const type_& value) {                              \
		TYPE_CHECK((*this), LIST);\
		add_tag({ value });                                         \
	} \
	type_& tag::as_##name_() {\
		TYPE_CHECK((*this), enum_type);\
		return data.v_##name_;\
	}

namespace jbt {

	tag::tag(): type(tag_type::NONE) {
		data.v_long = 0;
	}

	tag::tag(const tag_type& type): type(type) {
		setup_data();
	}

	tag::tag(tag_type&& type)
		:type(type) {
		setup_data();
	}

	tag& tag::operator=(const tag& other) {
		type = other.type;

		switch (other.type) {
		case tag_type::LIST:
			data.v_list = new list_t(*other.data.v_list);
			break;
		case tag_type::STRING:
			data.v_string = new std::string(*other.data.v_string);
			break;
		case tag_type::OBJECT:
			data.v_object = new object_t(*other.data.v_object);
			break;
		case tag_type::BYTE_ARRAY:
			data.v_byte_array = new byte_array_t(*other.data.v_byte_array);
			break;
		default:
			data = other.data;
		}
		return *this;
	}

	tag& tag::operator=(tag&& other) noexcept {
		data = other.data;
		type = other.type;
		other.data.v_long = 0;
		other.type = tag_type::NONE;
		return *this;
	}

	void tag::setup_data() {
		switch (type) {
		case tag_type::LIST:
			data.v_list = new list_t();
			break;
		case tag_type::STRING:
			data.v_string = new std::string();
			break;
		case tag_type::OBJECT:
			data.v_object = new object_t();
			break;
		case tag_type::BYTE_ARRAY:
			data.v_byte_array = new byte_array_t();
			break;
		default:
			data.v_ulong = 0; // fill 8 bytes = 0
		}
	}

	tag_type tag::get_type() const {
		return type;
	}

	tag::tag(const tag& other) {
		type = other.type;

		switch (other.type) {
		case tag_type::LIST:
			data.v_list = new list_t(*other.data.v_list);
			break;
		case tag_type::STRING:
			data.v_string = new std::string(*other.data.v_string);
			break;
		case tag_type::OBJECT:
			data.v_object = new object_t(*other.data.v_object);
			break;
		default:
			data = other.data;
		}
	}

	tag::tag(tag&& other) noexcept : data(other.data), type(other.type) {
		other.data.v_long = 0;
		other.type = tag_type::NONE;
	}

	tag::tag(const std::string& value) : type(tag_type::STRING) {
		data.v_string = new std::string(value);
	}

	tag::tag(std::string&& value) noexcept : type(tag_type::STRING) {
		data.v_string = new std::string(std::move(value));
	}

	tag::tag(const byte_array_t& value) : type(tag_type::BYTE_ARRAY) {
		data.v_byte_array = new byte_array_t(value);
	}

	tag& tag::get_tag(const std::string& name) const {
		TYPE_CHECK((*this), OBJECT);
		auto&& it = data.v_object->find(name);
		assert(it != data.v_object->end() && "Tag not found");
		return it->second;
	}

	void tag::set_tag(const std::string& name, const tag& value) {
		TYPE_CHECK((*this), OBJECT);
		assert(name.size() <= 255 && "Maximum name length is 255");
		assert(!name.empty() && "Empty name is not allowed");
		(*(data.v_object))[name] = value;
	}

	void tag::set_tag(const std::string& name, tag&& value) {
		TYPE_CHECK((*this), OBJECT);
		assert(name.size() <= 255 && "Maximum name length is 255");
		assert(!name.empty() && "Empty name is not allowed");
		(*(data.v_object))[name] = std::move(value);
	}

	std::string& tag::get_string(const std::string& name) const {
		TYPE_CHECK((*this), OBJECT);
		tag& a_tag = get_tag(name);
		TYPE_CHECK(a_tag, STRING);
		return a_tag.as_string();
	}

	std::string& tag::as_string() {
		TYPE_CHECK((*this), STRING);
		return *data.v_string;
	 }

	void tag::set_string(const std::string& name, const std::string& value) {
		TYPE_CHECK((*this), OBJECT);
		assert(value.size() <= 65535 && "Maximum length of string is 65535");
		set_tag(name, tag(value));
	}

	void tag::set_string(const std::string& name, std::string&& value) {
		TYPE_CHECK((*this), OBJECT);
		assert(value.size() <= 65535 && "Maximum length of string is 65535");
		set_tag(name, tag(std::move(value)));
	}

	byte_array_t& tag::get_byte_array(const std::string& name) const {
		TYPE_CHECK((*this), OBJECT);
		tag& a_tag = get_tag(name);
		TYPE_CHECK(a_tag, BYTE_ARRAY);
		return a_tag.as_byte_array();
	}

	void tag::set_byte_array(const std::string& name, const byte_array_t& value) {
		TYPE_CHECK((*this), OBJECT);
		set_tag(name, tag(value));
	}

	JBT_TAG_METHOD_IMPL(bool, bool, BOOL)
	JBT_TAG_METHOD_IMPL(byte, int8_t, BYTE)
	JBT_TAG_METHOD_IMPL(ubyte, uint8_t, UBYTE)
	JBT_TAG_METHOD_IMPL(short, int16_t, SHORT)
	JBT_TAG_METHOD_IMPL(ushort, uint16_t, USHORT)
	JBT_TAG_METHOD_IMPL(int, int32_t, INT)
	JBT_TAG_METHOD_IMPL(uint, uint32_t, UINT)
	JBT_TAG_METHOD_IMPL(long, int64_t, LONG)
	JBT_TAG_METHOD_IMPL(ulong, uint64_t, ULONG)
	JBT_TAG_METHOD_IMPL(float, float, FLOAT)
	JBT_TAG_METHOD_IMPL(double, double, DOUBLE)

	tag& tag::get_tag(const uint32_t& index) const {
		TYPE_CHECK((*this), LIST);
		assert(index < data.v_list->size() && "Out of bounds");
		return (*data.v_list)[index];
	}
	
	void tag::set_tag(const uint32_t& index, const tag& value) {
		TYPE_CHECK((*this), LIST);
		assert(index < data.v_list->size() && "Out of bounds");
		(*data.v_list)[index] = value;
	}

	void tag::set_tag(const uint32_t& index, tag&& value) {
		TYPE_CHECK((*this), LIST);
		assert(index < data.v_list->size() && "Out of bounds");
		(*data.v_list)[index] = std::move(value);
	}
	
	uint32_t tag::size() const {
		switch (type) {
		case jbt::tag_type::LIST:
			return data.v_list->size();
		case jbt::tag_type::STRING:
			return data.v_string->size();
		case jbt::tag_type::OBJECT:
			return data.v_object->size();
		default:
			assert(false && "Wrong tag type");
			return 0;
		}
	}

	void tag::reserve(const uint32_t& size) {
		TYPE_CHECK((*this), LIST);
		data.v_list->reserve(size);
	}

	void tag::resize(const uint32_t& size) {
		TYPE_CHECK((*this), LIST);
		data.v_list->resize(size);
	}

	void tag::set_string(const uint32_t& index, const std::string& value) {
		TYPE_CHECK((*this), LIST);
		STRING_CHECK(value);
		set_tag(index, new tag(value));
	}

	void tag::set_string(const uint32_t& index, std::string&& value) {
		TYPE_CHECK((*this), LIST);
		STRING_CHECK(value);
		set_tag(index, new tag(std::move(value)));
	}

	std::string& tag::get_string(const uint32_t& index) const {
		TYPE_CHECK((*this), LIST);
		tag& a_tag = get_tag(index);
		assert(index < data.v_list->size() && "Out of bounds");
		return a_tag.as_string();
	}

	void tag::set_byte_array(const uint32_t& index, const byte_array_t& value) {
		TYPE_CHECK((*this), LIST);
		set_tag(index, new tag(value));
	}

	byte_array_t& tag::get_byte_array(const uint32_t& index) const {
		TYPE_CHECK((*this), LIST);
		tag& a_tag = get_tag(index);
		assert(index < data.v_list->size() && "Out of bounds");
		return a_tag.as_byte_array();
	}

	void tag::add_tag(const tag& value) {
		TYPE_CHECK((*this), LIST);
		data.v_list->push_back(value);
	}

	void tag::add_tag(tag&& value) {
		TYPE_CHECK((*this), LIST);
		data.v_list->push_back(std::move(value));
	}

	void tag::add_string(const std::string& value) {
		TYPE_CHECK((*this), LIST);
		STRING_CHECK(value);
		add_tag({ value });
	}

	void tag::add_string(std::string&& value) {
		TYPE_CHECK((*this), LIST);
		STRING_CHECK(value);
		add_tag({ std::move(value) });
	}

	void tag::add_byte_array(const byte_array_t& value) {
		TYPE_CHECK((*this), LIST);
		add_tag(tag(value));
	}

	byte_array_t& tag::as_byte_array() {
		TYPE_CHECK((*this), BYTE_ARRAY);
		return *data.v_byte_array;
	}

	tag::~tag() {
		switch (type) {
		case tag_type::LIST:
			delete data.v_list;
			break;
		case tag_type::STRING:
			if (data.v_string != nullptr)
				delete data.v_string;
			break;
		case tag_type::OBJECT:
			delete data.v_object;
			break;
		case tag_type::BYTE_ARRAY:
			if (data.v_byte_array->is_owner) {
				delete[] data.v_byte_array->data;
				delete data.v_byte_array;
			}
			break;
		}
	}

	void tag::to_string(std::ostream& out) const {
		to_string(out, 0);
	}

	void tag::to_string(std::ostream& out, const int32_t& tabs) const {
		std::string tab = "";
		for (int i=0;i<tabs;++i) tab += "    ";
		std::string tab1 = tab + "    ";

		int i = 0;

		switch (type) {
		case jbt::tag_type::BOOL:
			out << (data.v_bool ? "true" : "false");
			break;
		case jbt::tag_type::LIST:
			out << "[\n";
			for (int i = 0; i < data.v_list->size(); ++i) {
				out << tab1;
				(*data.v_list)[i].to_string(out, tabs + 1);
				out << ((i == data.v_list->size() - 1) ? "\n" : ",\n");
			}
			out << tab << "]";
			break;
		case jbt::tag_type::STRING:
			out << '"' << *data.v_string << '"';
			break;
		case jbt::tag_type::OBJECT:
			out << "{\n";

			for (auto& [name, a_tag] : *data.v_object) {
				out << tab1 << name << ": ";
				a_tag.to_string(out, tabs + 1);
				out << ((i == data.v_object->size() - 1) ? "\n" : ",\n");
				++i;
			}
			out << tab << "}";
			break;
		case jbt::tag_type::BYTE:
			out << int32_t(data.v_byte) << "b";
			break;
		case jbt::tag_type::UBYTE:
			out << uint32_t(data.v_ubyte) << "ub";
			break;
		case jbt::tag_type::SHORT:
			out << data.v_short << "s";
			break;
		case jbt::tag_type::USHORT:
			out << data.v_ushort << "us";
			break;
		case jbt::tag_type::INT:
			out << data.v_int << "i";
			break;
		case jbt::tag_type::UINT:
			out << data.v_uint << "ui";
			break;
		case jbt::tag_type::LONG:
			out << data.v_long << "l";
			break;
		case jbt::tag_type::ULONG:
			out << data.v_ulong << "ul";
			break;
		case jbt::tag_type::FLOAT:
			out << std::setprecision(7) << data.v_float << "f";
			break;
		case jbt::tag_type::DOUBLE:
			out << std::setprecision(14) << data.v_double << "d";
			break;
		case jbt::tag_type::BYTE_ARRAY:
			out << "(" << data.v_byte_array->size << " ";
			out << ((data.v_byte_array->size == 1) ? "byte" : "bytes") << ")";
		default:
			break;
		}
	}

	std::ostream& operator<<(std::ostream& os, const tag_type& type) {
		
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const tag& a_tag) {
		a_tag.to_string(os);
		return os;
	}
}