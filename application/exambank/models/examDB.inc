<?php 

// Load database module
require_once 'Zend/Db.php';

/* This is a system to be coupled with a uwdir authentication scheme

*/

class ExamDB
{
	protected $_db;

	function __construct()
	{
		$params = array(
		    'host'           => '127.0.0.1',
		    'username'       => 'webuser',
		    'password'       => 'TroutIsGood',
		    'dbname'         => 'exambank_test'
		);

		$this->_db = Zend_Db::factory('Pdo_Mysql', $params);
	}

	/** getExam - Retrieve an exam from the exambank
	 *
	 * Queries the database for an exam with a given identifier.  False returned
	 * if the exam is not found.
	 *
	 * @param   id	The id for the exam desired
	 * @return		return a associative array with the exams information
	 */
	function getExam( $id )
	{
		$query = "SELECT *
			FROM exams_view
			WHERE id = ?";
		return $this->_db->fetchRow( $query, $id );
	}

	function getExams( $prefix, $code )
	{	$query = "SELECT *
			FROM exams_view
			WHERE prefix = ?
				AND code = ?";
		return $this->_db->fetchAll( $query, array( $prefix, $code ) );
	}

	/** getCourses - Retrieve a list of courses that contain exams
	 *
	 */
	function getCourses( $prefix = null, $code = null, $returnAll = false )
	{
		// TODO: respect returnAll parameter
		// If prefix isn't set, return a list of prefix
		if( $prefix == null )
		{	$query = "SELECT DISTINCT prefix FROM exams_view ORDER BY prefix";
			return $this->_db->fetchCol( $query );
		}

		// If code isn't set, return codes for the given prefix
		if( $code == null )
		{	$query = "SELECT DISTINCT code FROM exams_view WHERE prefix = ? ORDER BY code";
			return $this->_db->fetchCol( $query, $prefix );
		}

		// If both prefix and code are set, return course title
		$query = "SELECT title FROM courses WHERE prefix = ? AND code = ?";
		return $this->_db->fetchRow( $query, array( $prefix, $code ) );
	}
}
