#ifndef MOHRS_REGION_H
#define MOHRS_REGION_H

#include <string>
#include <map>

/**
    Medal of Honor - Rising Sun
*/
namespace MoHRS 
{
	/**
	 * @brief Enumerates different regions.
	 */
	enum class Regions : int8_t
	{
		Europe        = 1,  /**< America region code */
		North_America = 2,  /**< North America region code */
		South_America = 3,  /**< South America region code */
		Africa        = 4,  /**< Africa region code */
		Asia          = 5,  /**< Asia region code */
		Oceania       = 6,  /**< Oceania region code */
		Moon          = 7,  /**< Moon region code */
		Mars          = 8,  /**< Mars region code */
		Unknown       = -1, /**< Unknown region code */
	};

	/**
	 * @brief Map containing region names.
	 * @details This map associates each region enum value with its corresponding string name.
	 */
	extern std::map<Regions, std::string> RegionNames;
}

#endif // MOHRS_REGION_H
