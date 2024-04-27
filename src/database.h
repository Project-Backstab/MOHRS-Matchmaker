#ifndef DATABASE_H
#define DATABASE_H

#include <mutex>

class MYSQL;
class MYSQL_STMT;
class MYSQL_BIND;

class Database
{
	private:
		/**
		 * @brief Pointer to the MySQL database connection.
		 * 
		 * This pointer represents the connection to the MySQL database.
		 * It is used internally by the Database class to perform database operations.
		 */
		MYSQL*              _connection;

		/**
		 * @brief Mutex for thread-safe access to the database connection.
		 * 
		 * This mutable mutex is used to ensure thread safety when accessing
		 * the database connection represented by _connection. It is mutable
		 * to allow locking and unlocking operations even on const member functions.
		 */
		mutable std::mutex  _mutex;
		
	public:
		Database();
		~Database();
		
		/*
			Queries
		*/

		

		/*
			Events
		*/
		/**
		 * @brief Event function triggered when the database is started.
		 * 
		 * This function serves as an event handler that triggers when the database is started.
		 * 
		 * @note This function does not return any value.
		 */
		void OnDatabaseStart();
	
	private:
		/**
		 * @brief Establishes a connection to the database.
		 * 
		 * This function establishes a connection to the database using the database settings stored in the global settings object.
		 * It initializes the MySQL connection object and attempts to connect to the database server. If the connection is successful,
		 * the connection object `_connection` is updated with the connection information.
		 * 
		 * @return True if the connection to the database is successfully established, false otherwise.
		 */
		bool _connect();

		/**
		 * @brief Initializes a MySQL statement object.
		 * 
		 * This function initializes a MySQL statement object using the provided pointer to a statement object.
		 * If the MySQL connection is no longer alive, it reconnects to the database before initializing the statement.
		 * 
		 * @param statement A pointer to a MySQL statement object to be initialized.
		 * @return True if the statement object is successfully initialized, false otherwise.
		 */
		bool _init(MYSQL_STMT** statement);

		/**
		 * @brief Prepares a MySQL statement with a query.
		 * 
		 * This function prepares a MySQL statement with the provided query string using the specified statement object.
		 * If the preparation of the statement fails, an error message is logged, and any associated result set is freed.
		 * 
		 * @param statement The MySQL statement object to be prepared.
		 * @param query The query string to be prepared.
		 * @return True if the statement is successfully prepared with the query, false otherwise.
		 */
		bool _prepare(MYSQL_STMT* statement, const std::string query);

		/**
		 * @brief Prepares a MySQL statement with a query and binds input parameters.
		 * 
		 * This function prepares a MySQL statement with the provided query string using the specified statement object.
		 * It also binds input parameters specified in the `input_bind` array to the prepared statement.
		 * If the preparation of the statement or binding of input parameters fails, an error message is logged, and the statement is closed.
		 * 
		 * @param statement The MySQL statement object to be prepared and bound.
		 * @param query The query string to be prepared.
		 * @param input_bind An array of MySQL bind structures representing the input parameters to be bound to the statement.
		 * @return True if the statement is successfully prepared, and input parameters are bound, false otherwise.
		 */
		bool _prepare(MYSQL_STMT* statement, const std::string query, MYSQL_BIND* input_bind);
		
		/**
		 * @brief Executes a prepared MySQL statement.
		 * 
		 * This function executes a prepared MySQL statement using the specified statement object.
		 * If the execution of the statement fails, an error message is logged, and any associated result set is freed.
		 * 
		 * @param statement The MySQL statement object to be executed.
		 * @return True if the statement is successfully executed, false otherwise.
		 */
		bool _execute(MYSQL_STMT* statement);

		/**
		 * @brief Executes a prepared MySQL statement with output binding.
		 * 
		 * This function executes a prepared MySQL statement using the specified statement object.
		 * It also binds output parameters specified in the `output_bind` array to the executed statement.
		 * If the execution of the statement or binding of output parameters fails, an error message is logged,
		 * and the statement is closed.
		 * 
		 * @param statement The MySQL statement object to be executed and bound with output parameters.
		 * @param output_bind An array of MySQL bind structures representing the output parameters to be bound to the statement.
		 * @return True if the statement is successfully executed and output parameters are bound, false otherwise.
		 */
		bool _execute(MYSQL_STMT* statement, MYSQL_BIND* output_bind);
};

#endif // DATABASE_H
