<?php 

// Load database module
require_once 'Zend/Db.php';

class PolicyDB
{
	protected $_db;

	function __construct()
	{
		$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
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
	function getPolicy( $id, $date = null )
	{	if( $date != null )
		{	if( $id > 0 )
			{	$query = "SELECT policies_index.id, policies.name, policies.approved, policies.content
					FROM policies, policies_index
					WHERE policies.id = policies_index.policyId
						AND policies_index.id = ?
						AND policies.approved = ?";
			} else
			{	$query = "SELECT policies_index.id, policies.name, policies.approved, policies.content
					FROM policies, policies_index
					WHERE policies.id = policies_index.policyId
						AND policies.name = ?
						AND policies.approved = ?";
			}
			return $this->_db->fetchRow( $query, array($id, $date) );
		}

		if( $id > 0 )
		{	$query = "SELECT *
				FROM policies_current
				WHERE num = ?";
			return $this->_db->fetchRow( $query, array($id) );
		}else
		{	$query = "SELECT *
				FROM policies_current
				WHERE name = ?";
			return $this->_db->fetchRow( $query, array($id) );
		}
	}

	/** getRevisions - Returns a list of dates that this policy was changed
	 *
	 */
	function getRevisions( $id )
	{	$query = "SELECT policies.approved
			FROM policies, policies_index
			WHERE policies_index.id = ?
				AND policies_index.policyId = policies.id";
		return $this->_db->fetchCol( $query, array($id) );
	}
}
