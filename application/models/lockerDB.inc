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
	function signup( $lockerNum, $info )
	{
		// TODO: check that the system is currently active

		// Determine if the user can claim the locker
		$query = "SELECT current_userId FROM lockers WHERE id = ?";
		$holder = $this->_db->fetchOne($query, $lockerNum);

		if( $holder && $holder != $info['user'] )
			return false;

		// Check that the user doesn't have another locker
		$query = "SELECT id FROM lockers WHERE current_userId = ?";
		$locker = $this->_db->fetchOne($query, $info['user']);

		if( $locker && $locker != $lockerNum )
		{	$n = $this->_db->update('lockers', array("current_userId" => new Zend_Db_Expr('NULL'), "last_userId" => $info['user']), "id = {$locker}");
			if( $n != 1 )
				return false;
		}

		$data = array();

		$data['current_userId'] = $info['user'];

		// Set expiry date for the locker
		$mysql_format = "Y-m-d H:i:s";
		$expires = date( $mysql_format, $info['expires'] );
		$data['expires'] = date( $mysql_format, locker_expires() );

		if( $info['phone'] )
			$data['current_phone'] = $info['phone'];
		if( $info['combo'] )
			$data['current_combo'] = $info['combo'];

		$n = $this->_db->update('lockers', $data, "id = {$lockerNum}");

		if( $n == 1 )
			return true;
		return false;
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

