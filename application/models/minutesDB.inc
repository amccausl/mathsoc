<?php 

// Load database module
require_once 'Zend/Db.php';

class MinutesDB
{
	protected $_db;

	function __construct()
	{
		$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	function getMinutes( $id = null )
	{	if( $id )
		{	$query = "SELECT *
					  FROM minutes
					  WHERE id = ?";
			$minutes = $this->_db->fetchRow( $query, $id );
		}else
		{	$query = "SELECT *
					  FROM minutes
					  ORDER BY minutes.term DESC, minutes.meeting_number
					  LIMIT 100";
			$results = $this->_db->fetchAll( $query );

			$minutes = array();
			foreach( $results as $row )
			{	if( !isset($minutes[$row['term']]) )
				{	$minutes[$row['term']] = array();
				}
				$minutes[$row['term']][$row['id']] = $row;
			}
		}
		return $minutes;
	}
}
