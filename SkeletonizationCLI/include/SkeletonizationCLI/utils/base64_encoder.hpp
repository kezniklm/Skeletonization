#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace cli::utils
{
	/**
	 * @brief Utility class for Base64 encoding.
	 *
	 * Provides static methods for encoding binary data to Base64 strings.
	 * Follows Single Responsibility Principle by separating encoding
	 * concerns from export logic.
	 */
	class base64_encoder
	{
	public:
		/**
		 * @brief Encode binary data to Base64.
		 * @param data Input binary data.
		 * @param output Output string to receive Base64 encoded data.
		 * @return true always (encoding cannot fail for valid input).
		 */
		static bool encode(const std::vector<std::uint8_t>& data, std::string& output);

		/**
		 * @brief Encode binary data to Base64.
		 * @param data Pointer to input data.
		 * @param size Size of input data in bytes.
		 * @param output Output string to receive Base64 encoded data.
		 * @return true always.
		 */
		static bool encode(const std::uint8_t* data, std::size_t size, std::string& output);

		/**
		 * @brief Encode binary data to Base64 and return as string.
		 * @param data Input binary data.
		 * @return Base64 encoded string.
		 */
		[[nodiscard]] static std::string encode(const std::vector<std::uint8_t>& data);

		/**
		 * @brief Estimate the output size for Base64 encoding.
		 * @param input_size Size of input data in bytes.
		 * @return Size of the Base64 encoded output.
		 */
		[[nodiscard]] static constexpr std::size_t encoded_size(const std::size_t input_size) noexcept
		{
			return 4 * ((input_size + 2) / 3);
		}

	private:
		static constexpr char encoding_table[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	};

	/**
	 * @brief Case-insensitive string comparison.
	 * @param a First string.
	 * @param b Second string.
	 * @return true if strings are equal ignoring case.
	 */
	[[nodiscard]] bool equals_ignore_case(std::string_view a, std::string_view b) noexcept;
}
