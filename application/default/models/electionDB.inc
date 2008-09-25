<?php 

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class ElectionDB
{
	// Return a list of elections the given user is eligable to vote in
	function getElections( $userId )
	{	$db = Zend_Db::factory('Pdo_Mysql', Zend_Registry::getInstance()->get('config')->db);

		$query = "SELECT holders.position
			  FROM holders, terms
			  WHERE holders.term = terms.current_term
			    AND holders.unitId = ?
			    AND holders.userid = ?;";

		return $db->fetchAll($query, $unit, $userid);
	}
}
