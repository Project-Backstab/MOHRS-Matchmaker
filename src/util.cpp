#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <random>
#include <chrono>

#include <util.h>

// Util::Buffer

bool Util::Buffer::ReadString(const std::vector<char>& buffer, size_t& offset, std::string &str)
{	
	str.clear();
	
	for(;offset < buffer.size(); offset++)
	{
		if(buffer[offset] == 0x0)
		{
			offset++;
			return true;
		}
		
		str += buffer[offset];
	}
	
	return false;
}

bool Util::Buffer::ReadString(const std::vector<unsigned char>& buffer, size_t& offset, std::string &str)
{
	std::vector<char> buffer2(buffer.begin(), buffer.end());
	
	return Util::Buffer::ReadString(buffer2, offset, str);
}

std::string Util::Buffer::ToString(const std::vector<char>& buffer)
{
	std::string s;
	
	for(char v : buffer)
	{
		if((v >= 32 && v <= 126) || (v == 9))
		{
			s.push_back(v);
		}
		else
		{
			s.push_back('.');
		}
	}
	
	return s;
}

std::string Util::Buffer::ToString(const std::vector<unsigned char>& buffer)
{
	std::vector<char> buffer2(buffer.begin(), buffer.end());
	
	return Util::Buffer::ToString(buffer2);
}

//Util::Time

std::string Util::Time::GetNowDateTime(const std::string& format)
{
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm* timeInfo = std::localtime(&time);

	std::ostringstream oss;
	oss << std::put_time(timeInfo, format.c_str());
	
	return oss.str();
}

std::string Util::Time::GetTimeZone()
{
	// Fetch and append the time zone information dynamically
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(now);
	struct tm* local_time = std::localtime(&current_time);

	char timezone_buffer[10]; // Assuming timezone abbreviation is less than 10 characters
	strftime(timezone_buffer, sizeof(timezone_buffer), "%Z", local_time);

	// Construct a std::string from the C-style string
	std::string timezone_string(timezone_buffer);

	return timezone_string;	
}

// Util::Url

void Util::Url::GetElements(const std::string& url, std::string& url_base, Util::Url::Variables& url_variables)
{
	std::stringstream input, input2;
	std::string str_url_variables, url_variable, key, value;
	
	input.str(url);
	
	// Get url_base
	std::getline(input, url_base, '?');
	
	// Get url variables
	if(std::getline(input, str_url_variables, '?'))
	{
		input.clear();
		input.str(str_url_variables);
		
		// Get get url variable
		while(std::getline(input, url_variable, '&'))
		{
			input2.clear();
			input2.str(url_variable);
			
			// Get key
			std::getline(input2, key, '=');
			
			// Get value
			if(std::getline(input2, value, '='))
			{
				url_variables.insert({ key, Util::Url::Decode(value) });
			}
		}
	}
}

std::string Util::Url::Encode(const std::string& str)
{
    std::string new_str = "";
    char c;
    int ic;
    const char* chars = str.c_str();
    char bufHex[10];
    int len = strlen(chars);

    for(int i = 0; i < len; i++)
	{
        c = chars[i];
        ic = c;
		
        // uncomment this if you want to encode spaces with +
        /*if (c==' ') new_str += '+';   
        else */
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			new_str += c;
		}
        else
		{
            sprintf(bufHex,"%X",c);
            
			if(ic < 16) 
			{
                new_str += "%0"; 
			}
            else
			{
                new_str += "%";
			}
			
            new_str += bufHex;
        }
    }
	
    return new_str;
 }

std::string Util::Url::Decode(const std::string& str)
{
	std::string ret;
	char ch;
	int i, ii, len = str.length();

	for (i=0; i < len; i++)
	{
		if(str[i] != '%')
		{
			if(str[i] == '+')
			{
				ret += ' ';
			}
			else
			{
				ret += str[i];
			}
		}
		else
		{
			sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
	}
	
	return ret;
}

// Util

std::string Util::addQuote(const std::string& value)
{
	return "\"" + value + "\"";
}

std::string Util::removeQuote(const std::string& value)
{
	std::string new_value = value;

	if (!new_value.empty())
	{
        if (new_value.front() == '\"')
		{
            new_value.erase(0, 1); // Remove first character
        }
        if (new_value.back() == '\"')
		{
            new_value.pop_back(); // Remove last character
        }
    }

	return new_value;
}

std::vector<std::string> Util::splitFavorite(const std::string& input)
{
    std::vector<std::string> favorites;
    std::istringstream iss(input);
    std::string part;

    while (std::getline(iss, part, ';'))
	{
        favorites.push_back(part);
    }

    return favorites;
}

