<?php

/***** announceDB.inc -- Version 0.1 *****
 * Writen by Alex McCausland
 *
 * This class is used to interface with the database to retrieve the list of
 * announcements to be displayed to the user.
 *
 * ChangeLog:
 *  2007-02-23 - 0.1
 *    Initial Release
 *
 * Future Improvements:
 *
 */

class AnnounceDB
{
	private $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	/** getAnnouncements - Retrieve the announcements that should be displayed
	 *
	 * Queries the database for current announcements
	 *
	 * @return		returns an array of announcements
	 */
	function getAnnouncements()
	{
		$query = "SELECT DATE_FORMAT(event_date, '%Y-%m-%d') as event_date, title, content
				  FROM announcement
				  WHERE (NOW() <= DATE_ADD(event_date,INTERVAL 1 DAY)
					AND DATE_SUB(event_date, INTERVAL 30 DAY) <= NOW())
					OR event_date IS NULL
				  ORDER BY event_date ASC";

		$events = $this->_db->fetchAll( $query );

		return $events;
	}
}

?>
