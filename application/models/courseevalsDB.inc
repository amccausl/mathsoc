<?php

/***** courseevalsDB.inc -- Version 0.1 *****
 * Writen by Joseph Collins
 *
 * This class is used to interface with the database to retrieve the list of
 * course evaluations to be displayed to the user.
 *
 * ChangeLog:
 *  2010-07-15 - 0.1
 *    Initial Release
 *
 * Future Improvements:
 *
 */

class CourseEvalsDB
{
  private $_db;

  function __construct()
  {	
    $this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
  }

  /** getEvaluations - Retrieve the evaluations that should be displayed
   *
   * Queries the database for evaluations
   *
   * @return		returns an array of evaluations
   */
  function getEvaluations()
  {
    $query = "SELECT id, term, prof_file, prof_type, ta_file, ta_type
				  FROM course_evals
				  ORDER BY term DESC";

    $events = $this->_db->fetchAll( $query );
    
    return $events;
  }

  function getTermEvaluation($term)
  {
    $query = "SELECT id, term, prof_file, prof_type, ta_file, ta_type
    				FROM course_evals
				WHERE term = ?
				ORDER BY term DESC";
    $events = $this->_db->fetchAll ($query, $term);
    return $events;
  }
}

?>
