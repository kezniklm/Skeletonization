#pragma once

#include <string>
#include <string_view>

enum class name_policy { underscore_namespaces, unqualified };


inline void erase_all(std::string& s, const std::string_view needle)
{
	for (std::size_t pos = 0; (pos = s.find(needle, pos)) != std::string::npos;)
	{
		s.erase(pos, needle.size());
	}
}

inline std::string strip_templates(const std::string_view sv)
{
	std::string out;

	int depth = 0;

	for (const auto c : sv)
	{
		if (c == '<')
		{
			++depth;
			continue;
		}
		if (c == '>')
		{
			if (depth) --depth;
			continue;
		}
		if (depth == 0)
		{
			out.push_back(c);
		}
	}
	return out;
}

inline void remove_whitespace(std::string& s)
{
	std::erase_if(s, [](const auto ch) { return std::isspace(ch); });
}

inline std::string sanitize_ident(const std::string_view sv)
{
	std::string out;

	for (const char ch : sv)
	{
		const unsigned char c = static_cast<unsigned char>(ch);
		out.push_back((ch == '_' || std::isalnum(c)) ? ch : '_');
	}

	if (!out.empty() && std::isdigit(static_cast<unsigned char>(out[0])))
	{
		out.insert(out.begin(), '_');
	}

	return out;
}

[[nodiscard]] inline std::string canonical_type(const std::string_view s)
{
	std::string tmp(s);

	erase_all(tmp, "class ");
	erase_all(tmp, "struct ");
	erase_all(tmp, "enum ");
	erase_all(tmp, "const ");
	erase_all(tmp, "volatile ");
	erase_all(tmp, "__ptr64 ");

	std::string out = strip_templates(tmp);

	remove_whitespace(out);

	return out;
}

[[nodiscard]] inline std::string pythonize_type_name(std::string_view full,
                                                     const name_policy policy = name_policy::unqualified)
{
	if (policy == name_policy::unqualified)
	{
		if (const auto pos = full.rfind("::"); pos != std::string_view::npos)
		{
			full = full.substr(pos + 2);
		}
	}

	std::string result;

	std::size_t pos = 0;

	while (true)
	{
		const std::size_t next = full.find("::", pos);
		const std::string_view part = next == std::string_view::npos ? full.substr(pos) : full.substr(pos, next - pos);

		if (!part.empty())
		{
			if (!result.empty())
			{
				result.push_back('_');
			}
			result += sanitize_ident(part);
		}

		if (next == std::string_view::npos)
		{
			break;
		}

		pos = next + 2;
	}

	if (!result.empty() && std::isdigit(static_cast<unsigned char>(result[0])))
	{
		result.insert(result.begin(), '_');
	}

	return result;
}
