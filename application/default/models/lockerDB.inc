<?php

//require_once( "database/mathsocDB.inc" );
require_once( "mathsocDB.inc" );
require_once( "mathsoc.inc" );

/***** lockerDB.inc -- Version 0.1 *****
 * Writen by Alex McCausland
 *
 * ChangeLog:
 *  2006-12-04 - 0.1
 *    Initial Release
 *
 * Future Improvements:
 *    - Make work
 *
 * Structure:
 *
 * The locker system has 1 table storing:
 *
 * The lockers table stored the lockers on a term by term basis.
 * The columns are:
 *   lockerNum	The actual number for the locker
 *   combo	The combo for this locker

  CREATE TABLE `lockers` (
    `lockerId`       INT(10) NOT NULL,
    `current_userId` CHAR(8),
    `current_phone`  CHAR(12),
    `last_userId`    CHAR(8),
    `last_phone`     CHAR(12),
    `expires`        DATETIME,
    `combo`          VARCHAR(10),
    PRIMARY KEY (`lockerId`)
  ) TYPE=MyISAM;

*/

class LockerDB extends MathSocDB
{
	private $database = "mathsoc";

	function __construct()
	{	parent::__construct( $this->database );
	}

/*** USER FUNCTIONS ***/
/*takes in a locker number and returns all information associated with said
*that locker in an associative array
*/
	function lookup( $lockerId )
	{	$query = "SELECT * FROM `lockers` WHERE `lockerId` = {$lockerId}";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}elseif( $row = mysql_fetch_assoc( $result ) )
		{	return $row;
		}
	}

	/** taken - Return the status of a given set of lockers
	 *
	 */
	function taken( $start, $end=-1 )
	{
		if( $end == -1 )
			$end = $start;

		$return = array();

		$query = "SELECT lockerId, current_userId FROM lockers WHERE lockerId >= $start AND lockerId <= $end ORDER BY lockerId";
		$query = mysql_query( $query, $this->connection );
		while( $row = mysql_fetch_row( $query ) )
		{	$return[$row[0]] = $row[1];
		}

		return $return;
	}

	/** signup - Enters the given info as the owner of the given locker
	 *
	 */
	function signup( $lockerNum, $info )
	{
		// Search for locker, if holder, expire_lockers()

		$mysql_format = "Y-m-d H:i:s";
		$expires = date( $mysql_format, $info['expires'] );

		$query = "UPDATE lockers\n";
		$query .= "SET current_userId = '{$info['user']}'";
		if( $info['phone'] )
			$query .= ", current_phone = '{$info['phone']}'";
		else
			$query .= ", current_phone = NULL";
		if( $info['combo'] )
			$query .= ", combo = '{$info['combo']}'";
		else
			$query .= ", combo = NULL";
		if( $info['expires'] )
			$query .= ", expires = '{$expires}'";
		else
			$query .= ", expires = NULL";
				
		$query .= "\nWHERE lockerId = '{$lockerNum}'";

		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}
		return true;
	}

	function renew( $lockerId )
	{	$mysql_format = "Y-m-d H:i:s";
		$expires = date( $mysql_format, locker_expires() );
		$query = "UPDATE lockers SET expires = '{$expires}' WHERE `lockerId` = {$lockerId}";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}
		return true;
	}

	function find_locker( $userId )
	{	$query = "SELECT lockerId FROM `lockers` WHERE `current_userId` = '{$userId}'";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}elseif( $row = mysql_fetch_array( $result ) )
		{	return $row[0];
		}else
		{	return false;
		}
	}

	function swap( $userId, $new_locker, $info )
	{	// Switch user's locker to new locker
		$cur_locker = find_locker( $userId );
		if( $cur_locker )
		{
			$taken = $this->taken( $new_locker );
			$taken = $taken[$new_locker];

			$query = "UPDATE lockers a, lockers b
				  SET a.current_userId = b.current_userId, a.current_phone = b.current_phone, a.combo = b.combo, a.expires = b.expires,
					b.current_userId = NULL, b.current_phone = NULL, b.combo = NULL, b.expires = NULL
				  WHERE a.lockerId = {$new_locker} AND b.lockerId = {$cur_locker}";
			if( !( $result = @ mysql_query( $query, $this->connection ) ) )
			{	return false;
			}
			return true;
		}else
		{	return false;
		}
	}
	/**
		takes in an info array with $info['phone'] and $info['combo']and a locker id $id
		and updates the database at locker number $id with the phone and combination data
	*/
	function update($id , $info )
	{	
		$info['phone'] = mysql_real_escape_string($info['phone']);
		$info['combo'] = mysql_real_escape_string($info['combo']);
	    preg_match_all("/[0-9]+/", $info['phone'], $phoneNums);
		$info['phone'] = implode($phoneNums[0]);
		$query = "UPDATE lockers SET current_phone = '{$info['phone']}', combo = '{$info['combo']}' WHERE lockerId = {$id}";
		$ret_val = mysql_query($query, $this->connection);
		return $ret_val;
	}

	function free( $lockerId )
	{	// Free up a locker for use by other users
		$query = "UPDATE lockers SET current_userId = NULL, current_phone = NULL, combo = NULL, expires = NULL WHERE lockerId = {$lockerId}";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}
		return true;
	}

/*** OFFICE MANAGER FUNCTIONS ***/

	// Should mostly be run by cron jobs, but must be well documented
	function expire_lockers()
	{	// Find all lockers with a expiry date that has passed
		// Transfer current to last and set expires to null
		$query = "UPDATE lockers
			  SET last_userid = current_userId, last_phone = current_phone, current_userId = null, current_phone = null, expires = null
			  WHERE expires <= NOW()";
	}

/*** DEMOGRAPHIC FUNCTIONS ***/

	/** getUsage - Retrieve some usage statistics for the system
	 *
	 * @return		Return a hash with some usage statistics
	 */
	function getUsage()
	{
	}
	
	function is_active()
	{	
		// Find the day things used to expire
		$start = ( ceil( (date("m") - 1) / 4 ) % 3 ) * 4 + 1;
		$expires = mktime( 0, 0, 0, $start, 1, date("Y") + ($start == 1) );
		if( $start == 9 )
			$expires = strtotime( "next Monday", $expires );
		$expires = strtotime( "next Monday", $expires ) + 378000;

		//Is that day today?
		return date("z", $expires ) != date("z");
	}
}

function locker_expires()
{	// This method worked in all cases I tested (2007,2008)
	// Not garenteed correct!

	// Determine the month that starts the next term
	$start = ( ceil( date("m") / 4 ) % 3 ) * 4 + 1;

	// If January, increment year
	$expires = mktime( 0, 0, 0, $start, 1, date("Y") + ($start == 1) );

	// If September, first week is frosh week
	if( $start == 9 )
		$expires = strtotime( "next Monday", $expires );

	// Determine the end of the first week of school
	$expires = strtotime( "next Monday", $expires ) + 378000;

	return $expires;
}

if( eregi( basename(__FILE__), $_SERVER['PHP_SELF'] ) )
{
	print( "<pre>" );
	$database = new LockerDB();

	$locker = $database->find_locker("amccausl");
	print( "\nlocker number for amccausl:\n" );
	print_r( $locker );

	print( "\n locker info for $locker:\n" );
	$locker = $database->lookup( $locker );
	print_r( $locker );

	print( "\n expires on: $expires" );

	if( $database->free( "1" ) )
	{	print( "\nlocker #1 free" );
	}else
	{	print( "\nfailed to free locker #1" );
	}

	print( "</pre>" );
}
?>
