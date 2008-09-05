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
	function getGroups( $userid )
	{
		$query = "SELECT CONCAT(units.alias,'-',holders.position)
			  FROM holders, terms, units
			  WHERE holders.term = terms.current_term
			    AND holders.unitId = units.id
			    AND holders.userid = ?;";

		$result = $db->fetchAll($query, $userid);

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
	{	$user = array(
			"name" => "Alex McCausland",
			"profile" => "I'm a swinging dude",
			"current" => array(
				array( "name" => "Website Director" )),
			"past" => array(
				array( "name" => "Website Director" )));

		return $user;
	}
}
