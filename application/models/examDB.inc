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
	function getExam( $id )
	{
		$query = "SELECT e.id,
					CONCAT(c.prefix, ' ', c.code) AS course,
					c.prefix,
					c.code,
					e.term,
					e.type,
					e.exam_path,
					e.exam_type,
					e.solutions_path,
					e.solutions_type,
					e.approved
				  FROM exams e, courses c
				  WHERE e.id = ?
					AND e.courseId = c.id";
		return $this->_db->fetchRow( $query, $id );
	}

	function getExams( $prefix, $code )
	{
		$query = "SELECT e.id,
					CONCAT(c.prefix, ' ', c.code) AS course,
					c.prefix,
					c.code,
					e.term,
					e.type,
					e.exam_path,
					e.exam_type,
					e.solutions_path,
					e.solutions_type,
					e.approved
			FROM exams e, courses c
			WHERE c.id = e.courseId
				AND c.prefix = ?
				AND c.code = ?";
		return $this->_db->fetchAll( $query, array( $prefix, $code ) );
	}

	/** getCourses - Retrieve a list of courses that contain exams
	 *
	 */
	function getCourses( $prefix = null, $code = null, $returnAll = false )
	{
		// If prefix isn't set, return a list of prefix
		if( $prefix == null )
		{	if( $returnAll )
				$query = "SELECT DISTINCT prefix FROM courses ORDER BY prefix";
			else
				$query = "SELECT DISTINCT prefix FROM exams, courses WHERE courses.id = exams.courseId AND exams.visible = 1 ORDER BY prefix";
			return $this->_db->fetchCol( $query );
		}

		// If code isn't set, return codes for the given prefix
		if( $code == null )
		{	if( $returnAll )
				$query = "SELECT DISTINCT code FROM courses WHERE prefix = ? ORDER BY code";
			else
				$query = "SELECT DISTINCT code FROM exams, courses WHERE prefix = ? AND courses.id = exams.courseId AND exams.visible = 1 ORDER BY code";
			return $this->_db->fetchCol( $query, $prefix );
		}

		// If both prefix and code are set, return course title
		$query = "SELECT id, title FROM courses WHERE prefix = ? AND code = ?";
		return $this->_db->fetchRow( $query, array( $prefix, $code ) );
	}

	function getTypes()
	{	return array('Final', 'Midterm', 'Quiz', 'Test', 'Assignment');
	}

	function addExam( $exam )
	{	return $this->_db->insert('exams', $exam);
	}

	function getUnapprovedExams()
	{
		$query = "SELECT e.id,
					CONCAT(c.prefix, ' ', c.code) as course,
					e.term,
					e.uploader,
					e.type,
					e.number,
					e.practice,
					(e.exam_path IS NOT NULL) AS has_questions,
					(e.solutions_path IS NOT NULL) AS has_solutions
				  FROM exams e, courses c
				  WHERE e.visible = 1
					AND e.approved = 0
					AND e.courseId = c.id";
		$exams = $this->_db->fetchAll( $query );
		return $exams;
	}

	function getUnapprovedExam($id)
	{
		$query = "SELECT e.id,
					CONCAT(c.prefix, ' ', c.code) as course,
					e.term,
					e.uploader,
					e.type,
					e.number,
					e.practice,
					e.exam_path,
					e.exam_type,
					e.solutions_path,
					e.solutions_type,
					(e.exam_path IS NOT NULL) AS has_questions,
					(e.solutions_path IS NOT NULL) AS has_solutions
				  FROM exams e, courses c
				  WHERE e.visible = 1
					AND e.approved = 0
					AND e.courseId = c.id
					AND e.examId = ?";
		$exam = $this->_db->fetchRow( $query, $id );
		return $exam;
	}

	function approveExam($id)
	{	$n = $this->_db->update( 'exams', array('approved' => 1), "id = $id" );
		if( $n == 1 )
		{	return true;
		}
		return false;
	}

	function rejectExam($id)
	{	$n = $this->_db->update( 'exams', array('visible' => 0), "id = $id" );
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
