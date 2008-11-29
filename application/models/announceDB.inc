<?php

require_once("database/mathsocDB.inc");

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
 *    - don't show expired events
 *
 * Structure:
 *
 * The announcements table stores the announcements to give to people
 * The columns are:
 *   id		
 *   title	The title of the announcement
 *   event_date	
 *   content	

  CREATE TABLE announcement (
    id		int(11)		NOT NULL,
    title	varchar(255)	NOT NULL,
    event_date	datetime,
    content	text,

    PRIMARY KEY (id)
  ) type=MyISAM;

 */

class AnnounceDB extends MathSocDB
{
	private $database = "announcement_test";

	function __construct()
	{	parent::__construct( $this->database );
	}

	/** getAnnouncements - Retrieve the announcements that should be displayed
	 *
	 * Queries the database for current announcements
	 *
	 * @return		returns an array of announcements
	 */
	function getAnnouncements()
	{
		$query = "SELECT title, event_date, content
					 FROM announcement;";

		return this->queryAsHash( $query );
	}
}

?>
