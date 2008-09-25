<?php 

// Load database module
require_once 'Zend/Db.php';

class PolicyDB
{
	protected $_db;

	function __construct()
	{
		$params = array(
		    'host'           => '127.0.0.1',
		    'username'       => 'webuser',
		    'password'       => 'TroutIsGood',
		    'dbname'         => 'mathsoc'
		);

		$this->_db = Zend_Db::factory('Pdo_Mysql', $params);
	}

	/** getPolicies - Returns a list of the current policies
	 *
	 */
	function getPolicies()
	{	$query = "SELECT *
			FROM policies_current";
		return $this->_db->fetchAll( $query );
	}

	/** getPolicy - Returns a the most recent version of a given policy
	 *
	 */
	function getPolicy( $id )
	{
		return $this->_db->fetchRow( $query, array($id) );
	}

	/** getPolicy - Returns a given revision of a given policy
	 *
	 */
	function getPolicy( $id, $date )
	{
		return $this->_db->fetchRow( $query, array($id, $date) );
	}

	/** getRevisions - Returns a list of dates that this policy was changed
	 *
	 */
	function getRevisions( $id )
	{
		return $this->_db->fetchAll( $query, array($id) );
	}
}
