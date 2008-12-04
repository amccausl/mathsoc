<?php

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class UserDB
{
	private $_db;

	function __construct()
	{	$config = new Zend_Config_Ini('../config/main.ini', 'user_management');
		$this->_db = Zend_Db::factory($config->db);
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

	function getRefunds( $status = null, $term = null )
	{	if( isset( $term ) )
		{	$query = "SELECT userId
					  FROM refunds_users
					  WHERE term = ?
						AND status = ?";
			return $this->_db->fetchCol( $query, array($term, $status) );
		}

		$query = "SELECT userId
				  FROM refunds_users, terms
				  WHERE term = terms.current_term
					AND status = ?";
		return $this->_db->fetchCol( $query, $status );
	}

	function getTrend()
	{	$query = "SELECT *
				  FROM refunds_trends";
		return $this->_db->fetchAll($query);
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
