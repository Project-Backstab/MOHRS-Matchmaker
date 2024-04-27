#ifndef SERVICE_H
#define SERVICE_H

namespace Service
{
	/**
	 * @brief Enum representing different types of services.
	 * 
	 */
	enum Type
	{
		File_System,  /**< File system service. */
		Discord,      /**< Discord service. */
		None,         /**< No specific service type. */
	};
}

#endif // SERVICE_H