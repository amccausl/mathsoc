<?php 

// Load database module
require_once 'Zend/Db.php';

class BylawDB
{
	protected $_db;

	function __construct()
	{
		$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	/** getBylaws - Returns a list of the current bylaws
	 *
	 */
	function getBylaws()
	{	$query = "SELECT *
			FROM bylaws_current";
		return $this->_db->fetchAll( $query );
	}

	/** getBylaw - Returns a the most recent version of a given bylaw
	 *
	 */
	function getBylaw( $id, $date = null )
	{	if( $date != null )
		{	if( $id > 0 )
			{	$query = "SELECT bylaws_index.id, bylaws.name, bylaws.approved, bylaws.content
					FROM bylaws, bylaws_index
					WHERE bylaws.id = bylaws_index.bylawId
						AND bylaws_index.id = ?
						AND bylaws.approved = ?";
			} else
			{	$query = "SELECT bylaws_index.id, bylaws.name, bylaws.approved, bylaws.content
					FROM bylaws, bylaws_index
					WHERE bylaws.id = bylaws_index.bylawId
						AND bylaws.name = ?
						AND bylaws.approved = ?";
			}
			return $this->_db->fetchRow( $query, array($id, $date) );
		}

		if( $id > 0 )
		{	$query = "SELECT *
				FROM bylaws_current
				WHERE num = ?";
			return $this->_db->fetchRow( $query, array($id) );
		}else
		{	$query = "SELECT *
				FROM bylaws_current
				WHERE name = ?";
			return $this->_db->fetchRow( $query, array($id) );
		}
	}

	/** getRevisions - Returns a list of dates that this bylaw was changed
	 *
	 */
	function getRevisions( $id )
	{	$query = "SELECT bylaws.approved
			FROM bylaws, bylaws_index
			WHERE bylaws_index.id = ?
				AND bylaws_index.bylawId = bylaws.id";
		return $this->_db->fetchCol( $query, array($id) );
	}
}
