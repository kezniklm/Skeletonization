#include "SkeletonizationCLI/utils/base64_encoder.hpp"

#include <cctype>

namespace cli::utils
{
	constexpr char base64_encoder::encoding_table[];

	bool base64_encoder::encode(const std::vector<std::uint8_t>& data, std::string& output)
	{
		return encode(data.data(), data.size(), output);
	}

	bool base64_encoder::encode(const std::uint8_t* data,
	                            const std::size_t size,
	                            std::string& output)
	{
		if (size == 0)
		{
			output.clear();
			return true;
		}

		const std::size_t out_len = encoded_size(size);

		output.clear();
		output.reserve(out_len);

		std::size_t i = 0;

		while (i + 3 <= size)
		{
			const std::uint32_t b0 = data[i++];
			const std::uint32_t b1 = data[i++];
			const std::uint32_t b2 = data[i++];

			output.push_back(encoding_table[(b0 & 0xfc) >> 2]);
			output.push_back(encoding_table[((b0 & 0x03) << 4) | ((b1 & 0xf0) >> 4)]);
			output.push_back(encoding_table[((b1 & 0x0f) << 2) | ((b2 & 0xc0) >> 6)]);
			output.push_back(encoding_table[b2 & 0x3f]);
		}

		const std::size_t remaining = size - i;
		if (remaining == 1)
		{
			const std::uint32_t b0 = data[i];
			output.push_back(encoding_table[(b0 & 0xfc) >> 2]);
			output.push_back(encoding_table[(b0 & 0x03) << 4]);
			output.push_back('=');
			output.push_back('=');
		}
		else if (remaining == 2)
		{
			const std::uint32_t b0 = data[i];
			const std::uint32_t b1 = data[i + 1];
			output.push_back(encoding_table[(b0 & 0xfc) >> 2]);
			output.push_back(encoding_table[((b0 & 0x03) << 4) | ((b1 & 0xf0) >> 4)]);
			output.push_back(encoding_table[(b1 & 0x0f) << 2]);
			output.push_back('=');
		}

		return true;
	}

	std::string base64_encoder::encode(const std::vector<std::uint8_t>& data)
	{
		std::string result;
		encode(data, result);
		return result;
	}

	bool equals_ignore_case(const std::string_view a, const std::string_view b) noexcept
	{
		if (a.size() != b.size())
		{
			return false;
		}

		for (std::size_t i = 0; i < a.size(); ++i)
		{
			if (std::tolower(static_cast<unsigned char>(a[i])) !=
				std::tolower(static_cast<unsigned char>(b[i])))
			{
				return false;
			}
		}

		return true;
	}
}
