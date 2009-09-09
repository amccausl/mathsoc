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
  {	
    $this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
  }

  /** getAnnouncements - Retrieve the announcements that should be displayed
   *
   * Queries the database for current announcements
   *
   * @return		returns an array of announcements
   */
  function getAnnouncements($StartDate=false)
  {
    $fmtStartDate = "1901-01-01 00:00:01";

    if( $StartDate ) {
      $fmtStartDate = date( 'Y-m-d H:i:s', $StartDate );
    }

    $query = "SELECT DATE_FORMAT(event_date, '%Y-%m-%d') as event_date, title, content
				  FROM announcement
				  WHERE (NOW() >= DATE_ADD(event_date,INTERVAL 0 DAY)
					AND event_date > '$fmtStartDate')
					OR event_date IS NULL
				  ORDER BY event_date DESC";

    $events = $this->_db->fetchAll( $query );
    
    return $events;
  }
}

?>
