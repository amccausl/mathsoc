<?php 

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class UserDB extends Zend_Db_Table
{
	protected $_name = 'user_management';

	function getPositions( $userId, $unit = "mathsoc" )
	{	$db = Zend_Db::factory('Pdo_Mysql', Zend_Registry::getInstance()->get('config')->db);

		$query = "SELECT holders.position
			  FROM holders, terms
			  WHERE holders.term = terms.current_term
			    AND holders.unitId = ?
			    AND holders.userid = ?;";

		return $db->fetchAll($query, $unit, $userid);
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

		$result = $db->fetchAll($query, $userId);

		$groups = array();

		foreach( $result as $row )
		{	array_push( $groups, $row[0] );
		}

		return $groups;
	}

	function isMember( $userId )
	{
		$query = "SELECT *
			  FROM refunds, terms
			  WHERE refunds.userId = ?
			  AND refunds.term = terms.current_term;";

		$result = $db->fetchCol($query, $userId);

		if( empty( $result ) )
		{	
			$query = "SELECT *
				  FROM refunds, terms
				  WHERE refunds.userId = ?
				  AND refunds.term = terms.last_term;";
			$result = $db->fetchCol($query, $userId);

			if( empty( $result ) )
			{	return true;
			}
		}
		return false;
	}

	function getProfile( $userId )
	{	// Do an LDAP lookup of the user to determine what position they hold
		$ch = curl_init("http://kiwi.uwaterloo.ca/user/testLookup");
		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, "newLookup=".$userId."&commit=Find");
		curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.8.1.4) Gecko/20070515 Firefox/2.0.0.4 Test/0.1");
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		$kiwi = curl_exec($ch);
		curl_close($ch);

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

		$user = array(
			"name" => "Alex McCausland",
			"profile" => "I'm a swinging dude",
			"current" => array(
				array( "name" => "Website Director" )),
			"past" => array(
				array( "name" => "Website Director" )));

		return $user;
	}
}
