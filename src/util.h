#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief Forward declaration of the MYSQL_TIME structure.
 * 
 * This forward declaration allows the use of the MYSQL_TIME structure in contexts where only the structure name needs to be known,
 * such as declaring pointers or references to MYSQL_TIME objects, without the need to include the full definition of the structure.
 * 
 * MYSQL_TIME is typically used to represent date and time values in MySQL-related operations.
 */
struct MYSQL_TIME;

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
		 * @brief Get a formatted date and time from MYSQL_TIME.
		 * 
		 * This function formats a MYSQL_TIME structure into a date and time string.
		 * 
		 * @param datetime The MYSQL_TIME structure to format.
		 * @return The formatted date and time as a string.
		 */
		std::string GetDateTime(MYSQL_TIME datetime);

		/**
		 * @brief Get the current time zone.
		 * 
		 * This function retrieves the current time zone.
		 * 
		 * @return The current time zone as a string.
		 */
		std::string GetTimeZone();
	}
	
	std::string addQuote(const std::string& value);
	std::string removeQuote(const std::string& value);
	std::vector<std::string> splitFavorite(const std::string& input);
}

#endif // UTIL_H
