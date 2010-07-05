<?php 

/* This is a system to be coupled with a uwdir authentication scheme

*/

class ExamDB
{
	protected $_db;

	function __construct()
	{	$config = new Zend_Config_Ini('../config/main.ini', 'exambank');
		$this->_db = Zend_Db::factory($config->db);
	}

	/** getExam - Retrieve an exam from the exambank
	 *
	 * Queries the database for an exam with a given identifier.  False returned
	 * if the exam is not found.
	 *
	 * @param   id	The id for the exam desired
	 * @return		return an associative array with the exams information
	 */
	function getApprovedExam( $id )
	{
		$query = 'SELECT e.id,
					GROUP_CONCAT(CONCAT(c.prefix, \' \', c.code) SEPARATOR \'/\') AS course,
					c.prefix,
					c.code,
					e.term,
					e.uploader,
					e.type,
					e.number,
					e.practice,
					e.exam_path,
					e.exam_type,
					e.solutions_path,
					e.solutions_type
				  FROM exams e, courses c
				  WHERE e.id = ?
					AND e.courseId = c.id
				  GROUP BY e.id';
		return $this->_db->fetchRow( $query, $id );
	}

	function getExam($id)
	{
		$query = 'SELECT e.id,
					GROUP_CONCAT(CONCAT(c.prefix, " ", c.code) SEPARATOR "/") AS course,
					e.term,
					e.uploader,
					e.type,
					e.number,
					e.practice,
					e.exam_path,
					e.exam_type,
					e.solutions_path,
					e.solutions_type
				  FROM exams e, courses c
				  WHERE e.id = ?
					AND e.courseId = c.id
				  GROUP BY e.id';
		$exam = $this->_db->fetchRow( $query, $id );
		return $exam;
	}

	function getExams( $param, $page = 0 )
	{	$page_size = 30;

		$where = array();
		$params = array();

		// Add parameters to query
		if( isset($param['course_prefix']) )
		{	$where[] = 'c.prefix = ?';
			$params[] = $param['course_prefix'];
		}
		if( isset($param['course_code']) )
		{	$where[] = 'c.code = ?';
			$params[] = $param['course_code'];
		}
		if( isset($param['term']) )
		{	$where[] = 'e.term = ?';
			$params[] = $param['term'];
		}
		if( !isset( $param['status'] ) )
		{	$where[] = 'e.status != \'deleted\'';
		}elseif( $param['status'] == 'pending' )
		{	$where[] = 'e.status IS NULL';
		}elseif( $param['status'] == 'approved' )
		{	$where[] = 'e.status = \'approved\'';
		}elseif( $param['status'] == 'rejected' )
		{	$where[] = 'e.status = \'rejected\'';
		}

		$query = 'SELECT e.id,
					GROUP_CONCAT(CONCAT(c.prefix, " ", c.code) SEPARATOR "/") as course,
					c.prefix,
					c.code,
					e.term,
					e.uploader,
					e.type,
					e.number,
					e.practice,
					(e.exam_path IS NOT NULL) AS has_questions,
					(e.solutions_path IS NOT NULL) AS has_solutions,
					e.status
				  FROM exams e, courses c
				  WHERE e.courseId = c.id AND ' . implode( ' AND ', $where ) . '
				  GROUP BY e.id
				  ORDER BY e.term DESC
				  LIMIT ' . (intval($page) * $page_size) . ", {$page_size}";

		$exams = $this->_db->fetchAll( $query, $params );
		return $exams;
	}

	/** getCourses - Retrieve a list of courses that contain exams
	 *
	 */
	function getCourses( $prefix = null, $code = null, $returnAll = false )
	{
		// If prefix isn't set, return a list of prefix
		if( $prefix == null )
		{	if( $returnAll )
				$query = 'SELECT DISTINCT prefix FROM courses ORDER BY prefix';
			else
				$query = 'SELECT DISTINCT prefix FROM exams, courses WHERE courses.id = exams.courseId AND exams.status = \'approved\' ORDER BY prefix';
			return $this->_db->fetchCol( $query );
		}

		// If code isn't set, return codes for the given prefix
		if( $code == null )
		{	if( $returnAll )
				$query = 'SELECT DISTINCT code FROM courses WHERE prefix = ? ORDER BY code';
			else
				$query = 'SELECT DISTINCT code FROM exams, courses WHERE prefix = ? AND courses.id = exams.courseId AND exams.status = \'approved\' ORDER BY code';
			return $this->_db->fetchCol( $query, $prefix );
		}

		// If both prefix and code are set, return course title
		$query = 'SELECT * FROM courses WHERE prefix = ? AND code = ?';
		return $this->_db->fetchOne( $query, array( $prefix, $code ) );
	}

	function getTypes()
	{	return array('Final', 'Midterm', 'Quiz', 'Test', 'Assignment');
	}

	function addExam( $exam )
	{	return $this->_db->insert('exams', $exam);
	}

	function approveExam($id)
	{	$n = $this->_db->update( 'exams', array('status' => 'approved'), "id = $id" );
		if( $n == 1 )
		{	return true;
		}
		return false;
	}

	function rejectExam($id)
	{	$n = $this->_db->update( 'exams', array('status' => 'rejected'), "id = $id" );
		if( $n == 1 )
		{	return true;
		}
		return false;
	}

	function deleteExam($id)
	{	$n = $this->_db->update( 'exams', array('status' => 'deleted'), "id = $id" );
		if( $n == 1 )
		{	return true;
		}
		return false;
	}

	function updateExam($exam)
	{	$n = $this->_db->update( 'exams', $exam, "id = {$exam['id']}" );
		if( $n == 1 )
		{	return true;
		}
		return false;
	}
}
