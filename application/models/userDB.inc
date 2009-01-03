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
	{	$query = "SELECT positions.alias, positions.name, positions.description, positions.desired AS number, positions.email, positions.default_email 
				  FROM positions
				  WHERE positions.unitId = 1
					AND positions.alias = ?";

		$position = $this->_db->fetchRow( $query, $alias );

		if( empty( $position ) )
		{	return $position;
		}

		// This works because we restrict it to unit 1 (mathsoc)
		if( $position['email'] )
		{	$position['email'] = $position['alias'] . '@mathsoc.uwaterloo.ca';
		}else
		{	unset( $position['email'] );
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
		{	$current = array();
			$users = array();

			$names = split(',', $result['name']);
			$include_emails = split(',', $result['include_email']);
			$emails = split(',', $result['email']);

			for( $i = 0; $i < count( $names ); $i++ )
			{	if( $include_emails == 1 )
				{	array_push( $current, array("name" => $names[$i], "email" => $emails[$i]) );
				}else
				{	array_push( $current, array("name" => $names[$i]) );
				}
				array_push( $users, $names[$i] );
			}

			$terms[$result['term']] = $users;

			if( $result['term'] == $result['current_term'] )
			{	$position['current'] = $current;
				$position['current_count'] = count( $users );
			}
		}

		if( !isset( $position['number'] ) )
		{	$position['number'] = 1;
		}
		if( !isset( $position['current_count'] ) )
		{	$position['current_count'] = 0;
		}

		$position['holders'] = $terms;

		return $position;
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

	function getPositionsByCategory()
	{	$query = "SELECT p.category,
						p.alias,
						p.name,
						p.email,
						(SELECT GROUP_CONCAT(CONCAT_WS(':',volunteers.userId,volunteers.name) SEPARATOR ';')
						 FROM volunteers, holders, terms
						 WHERE terms.current_term = holders.term
							AND holders.userId = volunteers.userId
							AND p.alias = holders.position
						 GROUP BY holders.position) AS holders
				  FROM positions p
				  WHERE p.category != 'LIST'
				  ORDER BY p.name";
		$result = $this->_db->fetchAll($query);

		$return = array();

		foreach( $result as $row )
		{	if( !isset($return[$row['category']]) )
				$return[$row['category']] = array();

			$holders = array();
			if( $row['holders'] )
			{	$temp = explode(";", $row['holders']);
				foreach( $temp as $entry )
				{	$temp2 = explode(":", $entry);
					$holders[$temp2[0]] = $temp2[1];
				}
			}

			$row['holders'] = $holders;
			array_push( $return[$row['category']], $row );
		}

		return $return;
	}

	public function getUsers()
	{
		$query = "SELECT *
				  FROM volunteers_current
				  WHERE positions IS NOT NULL OR office_worker IS NOT NULL
				  ORDER BY volunteers_current.name";
		$result = $this->_db->fetchAll($query);

		$users = array();
		foreach( $result as $row )
		{
			$user = array(
				"name"	=> $row['name'],
				"positions" => array());

			if( $row['positions'] )
			{	$positions = explode( ";", $row['positions'] );
				foreach( $positions as $position )
				{	$position = explode( ":", $position );
					$user['positions'][$position[0]] = $position[1];
				}
			}

			if( $row['office_worker'] )
				$user['positions']['office worker'] = "Office Worker";
			$users[$row['userId']] = $user;
		}

		return $users;
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
		$user = array();

		$query = "SELECT *
				  FROM volunteers_current v
				  WHERE v.userId = ?";
		$row = $this->_db->fetchRow( $query, $userId );

		$user['name'] = $row['name'];
		$user['current'] = array();

		if( $row['positions'] )
		{	$positions = explode( ";", $row['positions'] );
			foreach( $positions as $position )
			{	$position = explode( ":", $position );
				$user['current'][$position[0]] = $position[1];
			}
		}

		if( $row['office_worker'] )
			$user['current']['office worker'] = "Office Worker";

		$query = "SELECT holders.term,
						GROUP_CONCAT(positions.alias SEPARATOR ';') AS alias,
						GROUP_CONCAT(positions.name SEPARAtor ';') AS name
				  FROM holders, positions, terms
				  WHERE holders.unitId = 1
					AND holders.position = positions.alias
					AND holders.userId = ?
					AND holders.term > (terms.last_term - 30)
				  GROUP BY holders.term
				  ORDER BY holders.term DESC";
		$results = $this->_db->fetchAll($query, $userId);

		$terms = array();

		foreach( $results as $row )
		{	$positions = array();

			$aliases = explode(';', $row['alias']);
			$names = explode(';', $row['name']);

			for( $i = 0; $i < count( $names ); $i++ )
			{	$positions[$aliases[$i]] = $names[$i];
			}

			$terms[$row['term']] = $positions;
		}
		$user['terms'] = $terms;

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
