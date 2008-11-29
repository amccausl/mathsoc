<?php

require_once("database/mathsocDB.inc");
require_once("mathsoc.inc");

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

class OfficeDB extends MathSocDB
{
	function __construct( $database = "mathsoc" )
	{	parent::__construct( $database );		// initializes a connection with the MathSoc database
	}

	/** getEmails - function for office managers to get workers email addresses
	*
	* @param	term	the term for which you want office worker emails
	*/
	function getEmails($term = null)
	{
		if(!$term)
		{	$term = current_term();
		}
		$return = array();

		$query = "SELECT r.userid, r.name, r.alternate_email
				FROM resourcelist r, office_workers w 
				WHERE w.term = $term AND w.userId = r.id
				ORDER BY r.name";
		$users = array();
		$result = mysql_query($query, $this->connection);
		while($row = mysql_fetch_array($result))
		{
			if(!$row['alternate_email'])
			{	$return[$row['name']] = $row['userid']."@uwaterloo.ca";	
			}else
			{	$return[$row['name']] = $row['alternate_email'];
			}
		}

		return $return;
	}

	/** missingHours - Determine which council members should have an hour and don't
	*/
	function missingHours()
	{
		$return = array();
		$query = "SELECT DISTINCT r.userid, r.name, r.alternate_email
			  FROM position p, link l, resourcelist r
			  WHERE p.category IN ('DIR', 'REP')
				AND l.res_id = r.id AND l.pos_id = p.id
				AND r.alternate_email != 'prez@mathsoc.uwaterloo.ca'
				AND r.id NOT IN (SELECT userId FROM office_workers WHERE term = " . current_term() . ")";
		$result = mysql_query($query, $this->connection);
		while( $row = mysql_fetch_array( $result ) )
		{
			if( !$row[2] )
			{	$return[$row[1]] = $row[0] . "@uwaterloo.ca";
			}else
			{	$return[$row[1]] = $row[2];
			}
		}
		$workers = $this->getEmails();
		foreach( $workers as $name => $email )
		{	unset( $return[$name] );
		}
		return $return;
	}
	
	/** getSchedule - Get the office worker schedule for the current term
	 *
	 * @param	term	The term for which you whoudl like a schedule
	 * @return		An array[day][hour][users]
	 */
	function getSchedule($term = null)
	{
		if( !$term )
			$term = current_term();

		$return = array();

		$query = "SELECT r.name, r.userid, r.alternate_email, h.day, h.start, h.duration
			  FROM office_hours h, office_workers w, resourcelist r
			  WHERE h.hourId = w.hourId AND w.term = $term AND w.userId = r.id
			  ORDER BY h.day, h.start";
		$result = mysql_query( $query, $this->connection );
		while( $row = mysql_fetch_assoc( $result ) )
		{
			if( !isset( $return[$row['day']] ) )
				$return[$row['day']] = array();

			$hour = substr( $row['start'], 0, 5 );
			if( !isset( $return[$row['day']][$hour] ) )
				$return[$row['day']][$hour] = array();

			$user = array();
			$user['id'] = $row['userid'];
			$user['name'] = $row['name'];
			if( $row['alternate_email'] )
				$user['email'] = $row['alternate_email'];
			else
				$user['email'] = $row['userid'] . "@uwaterloo.ca";

			array_push( $return[$row['day']][$hour], $user );
		}

		return $return;
	}
	
	/** signup - Signup a user to a given hour
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

if( eregi( basename(__FILE__), $_SERVER['PHP_SELF'] ) )
{
	// Load database
	print_r($database = new OfficeDB());

	print_r( $database->getSchedule() );

	// Test contents
	//$user['id'] = "amccausl";
	//$hour['id'] = 4;
	//$database->signup( $user, $hour );
}
?>
