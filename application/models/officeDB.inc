<?php

/***** officeDB.inc -- Version 0.1 *****
 *
 * ChangeLog:
 *  2006-11-10 - 0.1
 *    Initial Release
 *
 * Future Improvements:
 *    - Finish methods
 *
*/

class OfficeDB
{
	protected $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	/** getEmails - function for office managers to get workers email addresses
	*
	* @param	term	the term for which you want office worker emails
	*/
	function getEmails($term = null)
	{
		if(!$term)
		{	$query = "SELECT v.userId, v.name, e.email
					  FROM volunteers v, user_emails e, office_workers w, terms t
					  WHERE v.userId = w.userId
						AND e.userId = v.userId
						AND w.term = t.current_term
					  ORDER BY v.name";
			return $this->_db->fetchAll($query);
		}else
		{	$query = "SELECT v.userId, v.name, e.email
					  FROM volunteers v, user_emails e, office_workers w
					  WHERE v.userId = w.userId
						AND e.userId = v.userId
						AND w.term = ?
					  ORDER BY v.name";
			return $this->_db->fetchAll($query, array($term));
		}
	}

	/** missingHours - Determine which council members should have an hour and don't
	 */
	function missingHours()
	{
		$query = "SELECT DISTINCT h.userId, v.name, e.email
				  FROM position p, holders h, terms t, volunteers v, user_emails e
				  WHERE h.term = t.current_term
					AND e.userId = h.userId
					AND p.category IN ('DIR', 'REP')
					AND p.alias = h.position
					AND h.userId NOT IN (SELECT userId FROM office_workers, terms WHERE term = terms.current_term)";
		return $this->_db->fetchAll($query);
	}
	
	/** getSchedule - Get the office worker schedule for the current term
	 *
	 * @param	term	The term for which you whoudl like a schedule
	 * @return		An array[day][hour][users]
	 */
	function getSchedule($term = null)
	{
		if( !$term )
		{	$query = "SELECT v.name, v.userId AS id, e.email, h.day, h.start, h.duration
					  FROM office_hours h, office_workers w, terms t, user_emails e, volunteers v
					  WHERE v.userId = e.userId
						AND w.hourId = h.hourId
						AND w.term = t.current_term
						AND w.userId = e.userId
					  ORDER BY h.day, h.hourId";
			$result = $this->_db->fetchAll( $query );
		}else
		{	$query = "SELECT v.name, v.userId AS id, e.email, h.day, h.start, h.duration
					  FROM office_hours h, office_workers w, terms t, user_emails e, volunteers v
					  WHERE v.userId = e.userId
						AND w.hourId = h.hourId
						AND w.userId = e.userId
						AND w.term = ?
					  ORDER BY h.day, h.hours";
			$result = $this->_db->fetchAll( $query, array($term) );
		}

		$return = array();

		foreach( $result as $row )
		{
			if( !isset( $return[$row['day']] ) )
				$return[$row['day']] = array();

			$hour = substr( $row['start'], 0, 5 );
			if( !isset( $return[$row['day']][$hour] ) )
				$return[$row['day']][$hour] = array();

			$user = array();
			$user['id'] = $row['id'];
			$user['name'] = $row['name'];
			$user['email'] = $row['email'];

			array_push( $return[$row['day']][$hour], $user );
		}

		return $return;
	}
	
	/** TODO signup - Signup a user to a given hour
	 *
	 * This function assumes the user is already a volunteer, this check should
	 * happen on the front end.
	 *
	 * @param	user	
	 * @param	hour	
	 * @return		True if the operation was successful, false otherwise
	 */
	function signup($user, $hour)
	{
		// Create an array to be entered as a signup
		$signup = array();
		$signup['term'] = current_term();
		
		// Determine which user is signing up
		if(!isset($user['id']))
		{	$user['id'] = $_SESSION['username'];
		}
		if(!isset($user['id']))
		{	return false;
		}
		
		//check if user is in resource list table
		$query = "SELECT id FROM resourcelist WHERE userid='{$user['id']}'";
		if( !( $result = @ mysql_query( $query, $this->connection ) ) )
		{	
			// There has been a database error
			return false;

		}elseif( mysql_num_rows( $result ) == 1 )
		{
			// The user has volunteered before and is known
			$row = mysql_fetch_array( $result );
			$signup['userId'] = $row[0];

		}else
		{
			// The user is not yet known, must insert entry into resourcelist
			$insert = array();
			$insert['userid'] = $user['id'];
			$insert['name'] = $user['name'];
			
			// Add user to the system
			$this->insert( "resourcelist", $insert );

			// Retrieve the new users key from the database
			if( !( $result = @ mysql_query( $query, $this->connection ) ) )
			{	return false;
			}elseif( mysql_num_rows( $result ) != 1 )
			{	return false;
			}elseif( $row = mysql_fetch_array( $result ) )
			{	$signup['userId'] = $row[0];
			}else
			{	return false;
			}
		}

		// Determine which hour the user is signing up for
		if( isset( $hour['id'] ) )
		{	$signup['hourId'] = $hour['id'];
		}else
		{	$query = "SELECT hourId FROM office_hours WHERE day = '{$hour['day']}' AND start = '{$hour['time']}:00'";
			if( !( $result = @ mysql_query( $query, $this->connection ) ) )
			{	return false;
			}elseif( mysql_num_rows( $result ) != 1 )
			{	return false;
			}elseif( $row = mysql_fetch_array( $result ) )
			{	$signup['hourId'] = $row[0];
			}else
			{	return false;
			}
		}

		// Signup the office worker
		return $this->insert( "office_workers", $signup );
	}
}
