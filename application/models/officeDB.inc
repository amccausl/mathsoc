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

	/** getHours - return the hours held by a given user for this term
	 */
	function getHours( $userId, $term = null )
	{
		if(!$term)
	 	{	$query = "SELECT h.hourId, h.day, h.start, h.duration
					  FROM office_hours h, office_workers w, terms t
					  WHERE h.hourId = w.hourId
						AND w.term = t.current_term
						AND userId = ?
					  ORDER BY h.day, h.hourId";
            $result = $this->_db->fetchAll( $query, $userId );
		}else
		{	//TODO: add case
			return;
		}

		$response = array();
		$days = array("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday");
		foreach( $result as $row )
		{
			if( !isset( $response[$days[$row['day']]] ) )
				$response[$days[$row['day']]] = array();

			// TODO determine display hour
			$hour = $row['start'];

			$response[$days[$row['day']]][$row['hourId']] = $hour;
		}

		print_r( $response );
		return $response;
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
	
	/** signup - Signup a user to a given hour
	 *
	 * This function assumes the user is already a volunteer, this check should
	 * happen on the front end.
	 *
	 * @param	user	
	 * @param	hour	
	 * @return		True if the operation was successful, false otherwise
	 */
	function signup($userId, $hourId)
	{
		// Check that the user exists in the system
		$query = "select userId from volunteers where userId = ?";
		if( !$this->_db->fetchOne($query, $userId) )
		{
			require_once( 'userDB.inc' );

			// Ensure the user exists in the volunteers table
			$user = array( "userId" => $userId );
			$this->_db->insert('users', $user);

			$lookup = UserDB::lookup($userId);
			$user['name'] = $lookup['name'];

			$this->_db->insert('volunteers', $user);
		}
	
		// Create an array to be entered as a signup
		$signup = array(
			'term'	=> new Zend_Db_Expr('CONCAT((YEAR(CURDATE()) - 1900), FLOOR((MONTH(CURDATE()) - 1) / 4) * 4 + 1)'),
			'userId'=> $userId,
			'hourId'=> $hourId
		);

		try {
			return $this->_db->insert('office_workers', $signup);
		} catch (Exception $e) {
			return false;
		}
	}

	function dropHour( $userId, $hourId )
	{
		$where[] = "userId = '$userId'";
		$where[] = "hourId = '$hourId'";
		$where[] = "term = (SELECT current_term FROM terms)";
	
		$n = $this->_db->delete( 'office_workers', $where );
		if( $n == 1 )
			return true;
		return false;
	}
}
