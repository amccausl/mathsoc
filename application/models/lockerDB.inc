<?php

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
*/

class LockerDB
{
	protected $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	/** lookup - Returns all information for a given locker
	 */
	function lookup( $id )
	{	if( $id > 0 )
		{	$query = "SELECT * FROM `lockers` WHERE `id` = ?";
		}else
		{	$query = "SELECT * FROM `lockers` WHERE `current_userId` = ?";
		}
		return $this->_db->fetchRow($query, array($id) );
	}

	/** taken - Return the status of a given set of lockers
	 *
	 */
	function taken( $start, $end = null )
	{
		if( $end == null )
			$end = $start;

		$query = "SELECT id, (current_userId IS NOT NULL) AS taken FROM lockers WHERE id >= ? AND id <= ? ORDER BY id";
		return $this->_db->fetchPairs($query, array( $start, $end ) );
	}

	/** signup - Enters the given info as the owner of the given locker
	 *
	 */
/*
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
				
		$query .= "\nWHERE id = '{$lockerNum}'";

		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}
		return true;
	}

	function renew( $id )
	{	$mysql_format = "Y-m-d H:i:s";
		$data = array( "expires" => date( $mysql_format, locker_expires() ) );
		if( $id > 0 )
		{	return $this->_db->update('lockers', $data, "id = {$id}" );
		}else
		{	return $this->_db->update('lockers', $data, "current_userId = {$id}" );
		}
	}

	function find_locker( $userId )
	{	$query = "SELECT id FROM `lockers` WHERE `current_userId` = '{$userId}'";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	return false;
		}elseif( $row = mysql_fetch_array( $result ) )
		{	return $row[0];
		}else
		{	return false;
		}
	}

	/**
		takes in an info array with $info['phone'] and $info['combo']and a locker id $id
		and updates the database at locker number $id with the phone and combination data
	*/
/*
	function update($id , $info )
	{	
		$info['phone'] = mysql_real_escape_string($info['phone']);
		$info['combo'] = mysql_real_escape_string($info['combo']);
	    preg_match_all("/[0-9]+/", $info['phone'], $phoneNums);
		$info['phone'] = implode($phoneNums[0]);
		$query = "UPDATE lockers SET current_phone = '{$info['phone']}', combo = '{$info['combo']}' WHERE id = {$id}";
		$ret_val = mysql_query($query, $this->connection);
		return $ret_val;
	}

	function free( $lockerId )
	{	// Free up a locker for use by other users
		$query = "UPDATE lockers SET current_userId = NULL, current_phone = NULL, combo = NULL, expires = NULL WHERE id = {$lockerId}";
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

