#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <map>

namespace Util
{
	namespace Buffer
	{
		/**
		 * @brief Read a string from a buffer.
		 * 
		 * This function reads a string from a buffer at the specified offset.
		 * 
		 * @param buffer The buffer to read from.
		 * @param offset The offset in the buffer to start reading from.
		 * @param str The string to store the read value.
		 * @return True if the string was successfully read, false otherwise.
		 */
		bool ReadString(const std::vector<char>& buffer, size_t& offset, std::string &str);

		/**
		 * @brief Read a string from a buffer of unsigned chars.
		 * 
		 * This function reads a string from a buffer of unsigned chars at the specified offset.
		 * 
		 * @param buffer The buffer to read from.
		 * @param offset The offset in the buffer to start reading from.
		 * @param str The string to store the read value.
		 * @return True if the string was successfully read, false otherwise.
		 */
		bool ReadString(const std::vector<unsigned char>& buffer, size_t& offset, std::string &str);
		
		/**
		 * @brief Convert a buffer of chars to a string.
		 * 
		 * This function converts a buffer of chars to a string.
		 * 
		 * @param buffer The buffer of chars to convert.
		 * @return The resulting string.
		 */
		std::string ToString(const std::vector<char>& buffer);

		/**
		 * @brief Convert a buffer of unsigned chars to a string.
		 * 
		 * This function converts a buffer of unsigned chars to a string.
		 * 
		 * @param buffer The buffer of unsigned chars to convert.
		 * @return The resulting string.
		 */
		std::string ToString(const std::vector<unsigned char>& buffer);
	}
	
	namespace Url
	{
		/**
		 * @brief Type definition for URL variables.
		 */
		typedef std::map<std::string, std::string> Variables;
		
		/**
		 * @brief Extract elements from a URL.
		 * 
		 * This function extracts the URL base and variables from a given URL.
		 * 
		 * @param url The URL to extract elements from.
		 * @param url_base The base URL.
		 * @param url_variables The URL variables as a map.
		 */
		void GetElements(const std::string& url, std::string& url_base, Util::Url::Variables& url_variables);
		
		/**
		 * @brief Encode a string for URL.
		 * 
		 * This function encodes a string for URL usage.
		 * 
		 * @param str The string to encode.
		 * @return The encoded string.
		 */
		std::string Encode(const std::string& str);

		/**
		 * @brief Decode a URL-encoded string.
		 * 
		 * This function decodes a URL-encoded string.
		 * 
		 * @param str The URL-encoded string to decode.
		 * @return The decoded string.
		 */
		std::string Decode(const std::string& str);
	}
	
	namespace Time
	{
		/**
		 * @brief Get the current date and time.
		 * 
		 * This function retrieves the current date and time in the specified format.
		 * 
		 * @param format The format string for the date and time.
		 * @return The current date and time as a string.
		 */
		std::string GetNowDateTime(const std::string& format);

		/**
		 * @brief Get the current time zone.
		 * 
		 * This function retrieves the current time zone.
		 * 
		 * @return The current time zone as a string.
		 */
		std::string GetTimeZone();
	}
	
	/**
	 * @brief Adds quotation marks to the provided string.
	 * @param value The string to add quotation marks to.
	 * @return The string with quotation marks added.
	 */
	std::string addQuote(const std::string& value);

	/**
	 * @brief Removes quotation marks from the provided string.
	 * @param value The string to remove quotation marks from.
	 * @return The string with quotation marks removed.
	 */
	std::string removeQuote(const std::string& value);
	
	/**
	 * @brief Splits a string containing favorite items into a vector of strings.
	 * @param input The string containing favorite items.
	 * @return A vector of strings containing the individual favorite items.
	 */
	std::vector<std::string> splitFavorite(const std::string& input);
}

#endif // UTIL_H
