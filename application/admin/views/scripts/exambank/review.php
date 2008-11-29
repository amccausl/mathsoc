<?php

/***** exambank/review.php -- Version 0.1 *****
 *
 * This page allows the resources director, website, and VPA to approve exams for the exambank
 *
 * ChangeLog:
 *
 * Future Improvements:
 */

// Load template generator
require_once("mathsoc.inc");
require_once("exambank.inc");
$template = new ExamBankTMPL();
$template->secure();

// Load database functions
require_once("database/exambankDB.inc");
$database = new ExamDB();

// Determine if the user has admin rights
$groups = array_intersect( $admins, $_SESSION['groups'] );

if( $_POST['submit'] )
{
	$approve = array_keys( $_POST );
	$approve = $approve[0];
	$exam['examId'] = split( "_", $approve);
	$exam['examId'] = $exam['examId'][1];
	if( $database->approveExam( $exam ) )
		$body .= "EXAM APPROVED";
	else
		$body .= "EXAM APPROVAL FAILED";
}

if( empty( $groups ) )
{	print( "sod off" );
}else
{	$exams = $database->getUnapprovedExams();
	$body .= "
  <form action='{$_SERVER['SCRIPT_NAME']}' enctype='multipart/form-data' method='post'>
  <table>
    <caption>Unapproved Exams</caption>
    <thead><tr>
      <th scope='col'>Course</th>
      <th scope='col'>Term</th>
      <th scope='col'>Type</th>
      <th scope='col'>Uploader</th>
      <th scope='col'>Exam</th>
      <th scope='col'>Solutions</th>
      <th scope='col'>Actions</th>
    </tr></thead>
    <tbody>\n";
    
	foreach( $exams as $exam )
	{	
		$body .= "      <tr>\n";
		$courseName = "";
		foreach( $exam['course'] as $course )
		{	if( $courseName == "" )
				$courseName .= $course['prefix'] . $course['code'];
			else
				$courseName .= "/" . $course['prefix'] . $course['code'];
		}
		$body .= "      <th scope='row'>{$courseName}</th>\n";
			
		$term = number_to_term( $exam['term'] );
		$term = ($term['term'] . $term['year']);
		$body .= "      <td>{$term}</td>\n";
	
		if( $exam['number'] )
			$type = "{$exam['type']} {$exam['number']}";
		else
			$type = $exam['type'];
		
		$body .= "      <td>{$type}</td>\n";

		$body .= "      <td>{$exam['uploader']}</td>\n";
		
		if( $exam['file_path'] )
			$body .= "      <td><a href=\"exams.php?exam={$exam['examId']}\" target=\"_blank\" title=\"Download the $term exam\">View</a></td>\n";
		else
			$body .= "      <td></td>\n";
				
		if( $exam['sol_path'] )
			$body .= "      <td><a href=\"exams.php?sol={$exam['examId']}\" target=\"_blank\" title=\"Download the $term exam solution\">Download</a></td>\n";
		else
			$body .= "      <td></td>\n";

		$body .= "      <td><input type='submit' name='approve_{$exam['examId']}' value='Approve' /></td>\n";

		$body .= "      </tr>\n";
	}
				
	$body .= "
    </tbody>
  </table>
  <input type='hidden' name='submit' value='true'>
  </form>\n";

  	$template->setCurrentBlock( "MAIN_CONTENT" );
	$template->setVariable( "MAIN_HEADER", "Exambank" );
	$template->setVariable( "MAIN_BODY", $body );
}

?>
