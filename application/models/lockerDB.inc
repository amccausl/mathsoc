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
	function lookup( $id = null )
	{	if( $id == null )
		{	$query = "SELECT id, current_userId, current_phone, expires, last_userId, last_phone
					  FROM lockers
					  WHERE current_userId IS NOT NULL
					  ORDER BY id";
			return $this->_db->fetchAll( $query );
		}elseif( $id > 0 )
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

		$query = "SELECT id, (current_userId IS NOT NULL AND expires > now()) AS taken FROM lockers WHERE id >= ? AND id <= ? ORDER BY id";
		return $this->_db->fetchPairs($query, array( $start, $end ) );
	}

	/** signup - Enters the given info as the owner of the given locker
	 *
	 */
	function signup( $lockerNum, $info )
	{
		// Determine if the user can claim the locker
		$query = "SELECT current_userId FROM lockers WHERE id = ?";
		$holder = $this->_db->fetchOne($query, $lockerNum);
		$query = "SELECT expires FROM lockers WHERE id = ?";
		$expires = $this->_db->fetchOne($query, $lockerNum);

		if( $holder && $holder != $info['user'] && $expires > date("Y-m-d H:i:s") )
			return "The locker you are trying to sign up for is already held by someone else.  Please try another locker.";

		// Check that the user doesn't have another locker
		$query = "SELECT id FROM lockers WHERE current_userId = ? AND expires > '" . date("Y-m-d H:i:s") . "'";
		$locker = $this->_db->fetchOne($query, $info['user']);

		if( $locker && $locker != $lockerNum )
		{	$n = $this->_db->update('lockers', array("current_userId" => new Zend_Db_Expr('NULL'), "last_userId" => $info['user']), "id = {$locker}");
			if( $n != 1 )
				return "You are already signed up for locker #{$locker} and the system can't free that locker.  You are only allowed to have one locker, so please try to sign up again later.";
		}

		$data = array();
		$data['last_userId'] = $holder;

		$data['current_userId'] = $info['user'];

		// Set expiry date for the locker
		$mysql_format = "Y-m-d H:i:s";
		$data['expires'] = date( $mysql_format, locker_expires() );

		$data['current_phone'] = $info['phone'];
		$data['combo'] = $info['combo'];

		$n = $this->_db->update('lockers', $data, "id = {$lockerNum}");

		if( $n == 1 )
		{	if( $locker == $lockerNum )
			{	return "Your locker information has been updated successfully.";
			}else
			{	return "You have successfully signed up for locker #{$lockerNum}, which expires on " . $data['expires'] . ".";
			}
		}
		return "There was no change to the locker information in your request.  The information has not been updated";
	}

/*** DEMOGRAPHIC FUNCTIONS ***/

	/** getUsage - Retrieve some usage statistics for the system
	 *
	 * @return		Return a hash with some usage statistics
	 */
	function getUsage()
	{
		$mysql_format = "Y-m-d H:i:s";
		$then = date("U") - 2592000;

		// Find the day things used to expire
		$start = ( ceil( date("m", $then) / 4 ) % 3 ) * 4 + 1;
		$expires = mktime( 0, 0, 0, $start, 1, date("Y", $then) + ($start == 1) );
		if( $start == 9 )
			$expires = strtotime( "next Monday", $expires );
		$expires = strtotime( "next Monday", $expires ) + 378000;
		$expires = date( $mysql_format, $expires );

		$query = "SELECT
					(SELECT COUNT(id) FROM lockers WHERE current_userId IS NOT NULL) AS used_lockers,
					(SELECT COUNT(id) FROM lockers WHERE expires < NOW() AND current_userId IS NOT NULL) AS expired_lockers,
					(SELECT COUNT(id) FROM lockers WHERE expires = ? AND current_userId IS NOT NULL) AS expiring_lockers,
					(SELECT COUNT(id) FROM lockers) AS total_lockers";
		$usage = $this->_db->fetchRow( $query, $expires );

		$usage['isActive'] = $this->isActive();
		$usage['expiring'] = $expires;
		return $usage;
	}

/*** OFFICE MANAGER FUNCTIONS ***/

	/** getEmptyLockers - Return the lockers that are supposed to be empty.
	 */
	function getEmptyLockers()
	{
		// If the system is inactive, we can expire the old lockers
		if( !$this->isActive() )
		{
			$update = array(
				"last_userId"	=> new Zend_Db_Expr('current_userId'),
				"last_phone"	=> new Zend_Db_Expr('current_userId'),
				"current_userId"=> new Zend_Db_Expr('NULL'),
				"current_phone"	=> new Zend_Db_Expr('NULL')
			);

			$where = array(
				"expires <= NOW()",
				"current_userId IS NOT NULL"
			);

			$n = $this->_db->update('lockers', $update, $where);

			print( "$n lockers expired" );
		}

		// Since lockers were just expired, this list should be all available lockers.
		$query = "SELECT id, last_userId, last_phone, combo
				  FROM lockers
				  WHERE current_userId IS NULL
				  ORDER BY id";
		$lockers = $this->_db->fetchAll($query);

		return $lockers;
	}

	function isActive()
	{
		$then = date("U") - 2592000;

		// Find the day things used to expire
		$start = ( ceil( date("m", $then) / 4 ) % 3 ) * 4 + 1;
		$expires = mktime( 0, 0, 0, $start, 1, date("Y", $then) + ($start == 1) );
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

