<?php

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class ClubsDB
{
	private $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	public function getClubs()
	{	$query = "SELECT alias, name, url, room, phone, membership, description
				  FROM clubs, units
				  WHERE clubs.id = units.id";
		$clubs = $this->_db->fetchAll( $query );

		return $clubs;
	}

	public function getClub($id)
	{	if( $id > 0 )
		{	$query = "SELECT alias, name, url, room, phone, membership, description FROM clubs, units WHERE clubs.id = ? AND clubs.id = units.id";
		}else
		{	$query = "SELECT alias, name, url, room, phone, membership, description FROM clubs, units WHERE units.alias = ? AND clubs.id = units.id";
		}

		$club = $this->_db->fetchRow( $query, $id );
		return $club;
	}
}
