<?php

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class UserDB
{
	private $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	function getPosition( $alias )
	{	$query = "SELECT positions.alias, positions.name, positions.description, positions.desired AS number, positions.email, 
				  FROM positions
				  WHERE positions.unitId = 1
					AND positions.alias = ?";

		$position = $this->_db->fetchRow( $query, $alias );

		// This works because we restrict it to unit 1 (mathsoc)
		if( $position['email'] )
		{	$position['email'] = $position['alias'] . '@mathsoc.uwaterloo.ca';
		}

		$query = "SELECT holders.term,
						GROUP_CONCAT(volunteers.name SEPARATOR ',') AS name,
						GROUP_CONCAT(holders.email SEPARATOR ',') AS include_email,
						GROUP_CONCAT(user_emails.email SEPARATOR ',') AS email,
						terms.current_term
				  FROM holders, user_emails, volunteers, terms
				  WHERE holders.unitId = 1
					AND holders.position = ?
					AND holders.userId = user_emails.userId
					AND holders.userId = volunteers.userId
					AND holders.term > (terms.last_term - 30)
				  GROUP BY holders.term
				  ORDER BY holders.term DESC";
		$results = $this->_db->fetchAll($query, $alias);

		$terms = array();

		foreach( $results as $result )
		{	$users = array();

			$names = split(',', $result['name']);
			$include_emails = split(',', $result['include_email']);
			$emails = split(',', $result['email']);

			for( $i = 0; $i < count( $names ); $i++ )
			{	if( $include_emails == 1 )
				{	array_push( $users, array("name" => $names[$i], "email" => $emails[$i]) );
				}else
				{	array_push( $users, array("name" => $names[$i]) );
				}
			}

			$terms[$result['term']] = $users;

			if( $result['term'] == $result['current_term'] )
			{	$position['current'] = $users;
			}
		}

		$position['holders'] = $terms;

		print_r( $position );

		$position = array(
			"alias" => "prez",
			"name" => "president",
			"number" => 1,
			"email" => prez@mathsoc.uwaterloo.ca,
			"description" => "prez rocks",
			"current" => array(array("name" => "Chris Neal", "email" => "cneal@uwaterloo.ca")),
			"holders" => array(
				"Fall 2008" => array(array("name" => "Alex McCausland", "email" => "amccausl@uwaterloo.ca" ) ),
				"Spring 2008" => array(array("name" => "Clare Park", "email" => "s9park@uwaterloo.ca" ) )
				)
			);

		print_r( $position );
	}

	function getPositions( $userId = "", $unit = "mathsoc" )
	{
		if( $userId != "" )
		{	$query = "SELECT positions.alias, positions.name, positions.description
					  FROM holders, terms, positions
					  WHERE positions.alias = holders.position
						AND holders.term = terms.current_term
						AND holders.unitId = ?
						AND holders.userid = ?";
			return $this->_db->fetchAll($query, array($unit, $userid));
		}else
		{	$query = "SELECT positions.alias, positions.name, positions.description
					  FROM positions, units
					  WHERE units.id = positions.unitId
						AND units.alias = ?
					  ORDER BY positions.name";
			return $this->_db->fetchAll($query, $unit);
		}
	}

	function getElectionPositions()
	{	$query = "SELECT positions.alias, positions.name, positions.description
				  FROM positions, units
				  WHERE units.id = positions.unitId
					AND units.alias = 'mathsoc'
					AND (positions.category = 'EXC' OR positions.category = 'REP')
					AND positions.alias != ''
				  ORDER BY positions.name";
		return $this->_db->fetchAll($query);
	}

	// authenticate uses uwdir and mathsocs database to authenticate users
	//$_SESSION['authenticated'] - a BOOLEAN
	//$_SESSION['username'] - the username of the authenticated user
	//$_SESSION['groups'] - an array of groups for which the user is a member.
	function getGroups( $userId )
	{
		$query = "SELECT CONCAT(units.alias,'-',holders.position)
			  FROM holders, terms, units
			  WHERE holders.term = terms.current_term
			    AND holders.unitId = units.id
			    AND holders.userid = ?;";

		$result = $this->_db->fetchAll($query, $userId);

		$groups = array();

		foreach( $result as $row )
		{	array_push( $groups, $row[0] );
		}

		return $groups;
	}

	function isMember( $userId )
	{
		$query = "SELECT *
			  FROM refunds_users
			  WHERE userId = ?";

		$result = $this->_db->fetchCol($query, $userId);

		if( empty( $result ) )
		{	return true;
		}
		return false;
	}

	function getRefunds( $status = 'RECEIVED', $term = null )
	{
		if( isset( $term ) )
		{	$query = "SELECT userId
					  FROM refunds
					  WHERE term = ?
						AND status = ?";
			return $this->_db->fetchCol( $query, array($term, $status) );
		}

		$query = "SELECT userId
				  FROM refunds, terms
				  WHERE term = terms.current_term
					AND status = ?";
		return $this->_db->fetchCol( $query, $status );
	}

	function getTrend()
	{	$query = "SELECT *
				  FROM refunds_trends";
		return $this->_db->fetchAll($query);
	}

	function allowRefund($userId)
	{	// Set term to the last term
		$term = (date('Y') - 1900) * 10 + floor((date('m') - 1) / 4) * 4 + 1;
		if( $term % 10 == 1 )
		{	$term -= 2;
		}else
		{	$term -= 4;
		}

		// Update the entry in table to 'REGULAR'
		if( $this->_db->update( 'refunds',
				array( 'status' => 'REGULAR' ),
				array( "userId = '$userId'", "term = '$term'", "status = 'RECEIVED'" ) ) == 1 )
		{	return true;
		}else
		{	return false;
		}
	}

	function rejectRefund($userId, $reason)
	{	// Set the term 
		$term = (date('Y') - 1900) * 10 + floor((date('m') - 1) / 4) * 4 + 1;

		// Update the entry for this terms user to 'REJECTED'
		if( $this->_db->update( 'refunds',
				array( 'status' => 'REJECTED', 'reason' => $reason ),
				array( "status = 'REQUESTED'", "userId = '$userId'" ) ) )
		{	return true;
		}else
		{	return false;
		}
	}

	function getProfile( $userId )
	{
		$user = self::lookup( $userId );

		// TODO: lookup user from local database to return local data

		return $user;
	}

	public static function lookup( $userId )
	{	$user = array("userId" => $userId);

		// Do an LDAP lookup of the user to determine what position they hold
		$ch = curl_init("http://kiwi.uwaterloo.ca/user/testLookup");
		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, "newLookup=".$userId."&commit=Find");
		curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.8.1.4) Gecko/20070515 Firefox/2.0.0.4 Test/0.1");
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		$kiwi = curl_exec($ch);
		curl_close($ch);

		if( preg_match( '/<b>(.*)<\/b>/', $kiwi, $matches ) )
		{	$user['name'] = $matches[1];
		}else
		{	return false;
		}

		$start = strpos($kiwi, "LDAP:") + 6;
		$ldap = substr($kiwi, $start, strpos($kiwi, "</span>", $start) - $start);

		if (strpos($ldap, "CN=IdM-HR-faculty") !== false
		&& strpos($ldap, "CN=IdM-active") !== false)
			$user['position'] = "professor";

		if (strpos($ldap, "CN=IdM-student") !== false
		&& strpos($ldap, "CN=IdM-active") !== false)
			$user['position'] = "student";

		if (strpos($ldap, "CN=IdM-SA-Mathematics") !== false
		&& strpos($ldap, "CN=IdM-active") !== false)
			$user['faculty'] = "math";

		return $user;
	}
}
