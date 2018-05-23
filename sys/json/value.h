#pragma once


#include <sys/types/string.h>
#include <vector>
#include <map>
#include <sys/debugging/debug.h>


namespace sys
{

struct json_null_t {};
struct json_array_t {};
struct json_object_t {};

struct json_value
{
private:
	enum class kind_e
	{
		invalid,
		null,
		boolean,
		numeric,
		string,
		array,
		object
	};

public:

	json_value(const json_value&) = default;
	json_value(json_value&&) = default;
	json_value& operator=(const json_value&) = default;
	json_value& operator=(json_value&&) = default;

	json_value() : m_kind(kind_e::invalid) {}
	json_value(json_null_t) : m_kind(kind_e::null) {}
	json_value(json_array_t) : m_kind(kind_e::array) {}
	json_value(json_object_t) : m_kind(kind_e::object) {}
	json_value(bool b) : m_kind(kind_e::boolean), m_bool(b) {}
	json_value(int i) : m_kind(kind_e::numeric), m_num(i) {}
	json_value(const sys::char_t* const s) : m_kind(kind_e::string), m_str(s) {}
	json_value(sys::string s) : m_kind(kind_e::string), m_str(std::move(s)) {}

	bool is_valid() const { return m_kind != kind_e::invalid; }

	bool is_null() const { return m_kind == kind_e::null; }

	bool is_array() const { return m_kind == kind_e::array; }
	std::vector<json_value>& get_array() { ASSERT(is_array());  return m_array; }
	const std::vector<json_value>& get_array() const { ASSERT(is_array());  return m_array; }

	bool is_object() const { return m_kind == kind_e::object; }
	std::map<sys::string, json_value>& get_object() { ASSERT(is_object());  return m_object; }
	const std::map<sys::string, json_value>& get_object() const { ASSERT(is_object());  return m_object; }

	bool is_bool() const { return m_kind == kind_e::boolean; }
	bool try_bool(bool& out) const { out = m_bool; return is_bool(); }
	bool get_bool(bool def = false) const { bool ret; if (try_bool(ret) == false) ret = def; return ret; }

	bool is_numeric() const { return m_kind == kind_e::numeric; }
	bool try_numeric(int& out) const { out = m_num; return is_numeric(); }
	int get_numeric(int def = 0) const { int ret; if (try_numeric(ret) == false) ret = def; return ret; }

	bool is_string() const { return m_kind == kind_e::string; }
	bool try_string(sys::string& out) const { if (is_string()) { out = m_str; return true; } return false; }
	sys::string get_string(sys::string def = "") const { sys::string ret; if (try_string(ret) == false) ret = std::move(def); return ret; }

	const json_value& operator[](unsigned int i) const
	{
		if (is_array() && m_array.size() < i)
		{
			return m_array[i];
		}

		return invalid_json_node;
	}
	json_value& operator[](unsigned int i)
	{
		if (m_kind != kind_e::array)
		{
			m_array.clear();
		}

		m_kind = kind_e::array;
		int s = m_array.size();
		ASSERT(i < s);
		return m_array[i];
	}
	const json_value& operator[](const sys::string& kv) const
	{
		if (is_object() == false)
		{
			return invalid_json_node;
		}

		auto it = m_object.find(kv);
		if (it == m_object.end())
		{
			return invalid_json_node;
		}

		return it->second;
	}
	json_value& operator[](const sys::string& kv)
	{
		if (m_kind != kind_e::object)
		{
			m_object.clear();
		}
		m_kind = kind_e::object;
		return m_object[kv];
	}

private:
	kind_e m_kind;
	bool m_bool;
	int m_num;
	sys::string m_str;
	std::vector<json_value> m_array;
	std::map<sys::string, json_value> m_object;

	static const json_value invalid_json_node;
};


namespace detail
{
	json_value parse_value(const sys::char_t*& pos)
	{
		json_value rv;
		return rv;
	}

}

json_value json_parse(const sys::string& in)
{
	return json_value();
}

sys::string json_write(const json_value& v)
{
	if (v.is_null()) { return "null"; }
	if (v.is_bool()) { return v.get_bool() ? "true" : "false"; }
	if (v.is_numeric()) { return sys::to_string(v.get_numeric()); }
	if (v.is_string()) { return TEXT("\"") + v.get_string() + TEXT("\""); }

	sys::string rv;
	if (v.is_array())
	{
		auto array_values = v.get_array();
		if (array_values.size() == 0)
		{
			return TEXT("[]");
		}

		rv = TEXT("[");
		for (const auto& i : array_values)
		{
			rv += json_write(i);
			rv += ",";
		}
		rv.pop_back();
		rv += TEXT("]");

		return rv;
	}

	if (v.is_object())
	{
		auto object_values = v.get_object();
		if (object_values.size() == 0)
		{
			return TEXT("{}");
		}

		rv = TEXT("{");
		for (const auto& i : object_values)
		{
			rv += TEXT("\"") + i.first + TEXT("\":");
			rv += json_write(i.second);
			rv += TEXT(",");
		}
		rv.pop_back();
		rv += TEXT("}");

		return rv;
	}

	return "";
}

}
