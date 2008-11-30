<?php

/***** mathsocDB.inc -- Version 0.1 *****
 * Writen by Alex McCausland
 *
 * The purpose of this class is to abstract away the database from the rest of the website page.
 * It also helps keep malicious code from executing as only predefined functions can be called and
 * input is validated by the database code.
 * To use, just extend from MathSocDB for each function the database is needed for.  This file will
 * maintain global variables so updating and testing all the database code becomes easy.
 *
 * ChangeLog:
 *  2007-01-01 - 0.1
 *    Initial Release
 *
 * Future Improvements:
 *    - Add in input validation
 *    - Add error handling
 *    - Add in __destruct method to close out all connections
 */

class MathSocDB
{
        // General information for the database
        private $hostname = "localhost";
        private $username = "webuser";
        private $password = "TroutIsGood";

        // Holds a connection to the database for queries
        protected $connection;

	/** MathSocDB - Create a generic MathSoc database connection
	 * 
	 * @param   database	The name for the database to connect to
	 * @return		MathSocDB object with the protected 'connection'
	 */
        function __construct( $database )
        {       if( !( $this->connection = @ mysql_connect( $this->hostname, $this->username, $this->password ) ) )
                {       //there has been an error connecting to the database
			print( "There has been an error in making a database connection" );
                }

		// Select the database desired by for the connection
		if( !( mysql_select_db( $database, $this->connection ) ) )
		{	//Todo:  put error handling functions here
		}
        }

	/** queryAsHash - Run a given query and return the result as a hash
	 *
	 * @param   query	The sql query the user would like to run
	 * @param   numResults	The number of results desired, false if don't care
	 * @return		associative hash of the result of the query
	 *			false if database error occured, empty array if no results
	 */
	protected function queryAsHash( $query, $numResults=false )
	{
		// Todo: verify there is no malicious code in query

		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}
		
		$return = array();
		
		while( (!$numResults || count($return) < $numResults ) && $row = mysql_fetch_array( $result, MYSQL_ASSOC ) )
		{	array_push( $return, $row );
		}
		
		return $return;
	}

	protected function insert( $table, $data )
	{
		$fields = implode( ", ", array_keys( $data ) );
		$values = "'" . implode( "', '", array_values( $data ) ) . "'";

		$query = "INSERT INTO {$table}
			    ({$fields})
			  VALUES
			    ({$values});";

		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}

		return true;
	}
}

?>
