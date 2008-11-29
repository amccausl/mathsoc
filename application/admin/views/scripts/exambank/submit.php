<?php

/***** exambank/submit.php -- Version 1.1 *****
 *
 * This page allows authenticated users to submit exams to the exambank electronically
 *
 * ChangeLog:
 *
 * Future Improvements:
 *    - do file type translations for exams of an undesirable type
 */

// Load template generator
require_once("mathsoc.inc");
require_once("exambank.inc");
$template = new ExambankTMPL();
$template->secure();

// Load database functions
require_once("database/exambankDB.inc");
$database = new ExamDB();

if( isset( $_POST['submit'] ) )
{
	// Create exam object to add
	$exam = array();
	$exam['uploader'] = $_SESSION['username'];

	// Must validate all form info and translate to $exam

	// Check prefix exists and is valid
	if( !isset( $_POST['course_prefix'] ) )
	{	// Prefix has not been set, but is manditory
		$error .= "You must enter the prefix for the course.<br />";
	}elseif ( !sanitize( $_POST['course_prefix'], "course_prefix" ) )
	{	// Prefix is not a valid prefix
		$error .= "You must include a valid prefix<br />";
	}else
	{	if( !in_array( strtoupper( $_POST['course_prefix'] ), $database->getPrefixes() ) )
		{	// Given prefix is unknown
			$error .= "The prefix you have chosen is unknown.<br />";
		}else
		{	// Check number exists and is valid
			if( !isset( $_POST['course_number'] ) )
			{	// Course number has not been included
				$error .= "You must enter the number of the course.<br />";
			}elseif( !sanitize( $_POST['course_number'], "course_number" ) )
			{	// Course number included is not a course number
				$error .= "You must include a valid course number.<br />";
			}else
			{	// Ensure the course exists
				if( !$course = $database->getCourses( strtoupper( $_POST['course_prefix'] ), $_POST['course_number'] ) )
				{	// The course doesn't exist in the database
					$error .= "The course you have selected was not found in the database.<br />";
				}else
				{	$exam['courseId'] = $course['id'];
				}
			}
		}
	}

	// Check if term exists and is valid
	if( !isset( $_POST['term'] ) )
	{	$error .= "Must select the term for the exam<br />";
	}elseif( !ereg( "^[0-9]{4}$", $_POST['term'] ) && !ereg( "^[0-9]{3}$", $_POST['term'] ) )
	{	$error .= "The term you have selected is invalid<br />";
	}else
	{	$exam['term'] = $_POST['term'];
	}

	// Check if type exists and is valid
	if( !isset( $_POST['type'] ) )
	{	$error .= "You must select the type of the exam<br />";
	//}elseif( !ereg( "^(Assignment|Test|Final|Midterm)$", $_POST['type'] ) )
	//{	$error .= "The exam type you selected is invalid<br />";
	}else
	{	$exam['type'] = $_POST['type'];
	}

	// Add index if valid
	if( ereg( "^[0-9]+$", $_POST['index'] ) )
	{	$exam['index'] = $_POST['index'];
	}

	// Check status of practice checkbox
	if( $_POST['practice'] )
		$exam['practice'] = 1;		

	if( !isset( $error ) )
	{
		if( $database->getExam($exam) )
		{	// exam already exists in the system
			$error .= "The exam you are trying to submit already exists in the system<br />";
		}else
		{
			// Check if user has attempted to upload an exam or solution
			if( $_FILES['exam']['error'] !== UPLOAD_ERR_NO_FILE || $_FILES['solutions']['error'] !== UPLOAD_ERR_NO_FILE )
			{
				if( $_FILES['exam']['error'] === UPLOAD_ERR_OK )
				{	$uploadExam = hash_file( "md5", $_FILES['exam']['tmp_name'] );
				
					if( move_uploaded_file( $_FILES['exam']['tmp_name'], $examDir . $uploadExam ) )
					{	$exam['file_path'] = $uploadExam;
						$exam['file_type'] = $_FILES['exam']['type'];
						chmod( $examDir . $uploadExam, 0575 );
					}
				}elseif( $_FILES['exam']['error'] === UPLOAD_ERR_INI_SIZE || $_FILES['exam']['error'] === UPLOAD_ERR_FORM_SIZE )
				{	// File is too large to be uploaded
					$error .= "The exam you are submitting is too large to be submitted in this manor.  We are very sorry of the inconvience.  Please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
				}elseif( $_FILES['exam']['error'] === UPLOAD_ERR_PARTIAL )
				{	$error .= "There was an error in the transmission of your exam.  Please try again.  If the problem persists, please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
				}else
				{	// No Exam submitted
				}

				if( $_FILES['solutions']['error'] === UPLOAD_ERR_OK )
				{	$uploadSol = hash_file( "md5", $_FILES['solutions']['tmp_name'] );
				
					if( move_uploaded_file( $_FILES['solutions']['tmp_name'], $examDir . $uploadSol ) )
					{	$exam['sol_path'] = $uploadSol;
						$exam['sol_type'] = $_FILES['solutions']['type'];
						chmod( $examDir . $uploadSol, 0575 );
					}
				}elseif( $_FILES['solutions']['error'] === UPLOAD_ERR_INI_SIZE || $_FILES['solutions']['error'] === UPLOAD_ERR_FORM_SIZE )
				{	// File is too large to be uploaded
					$error .= "The solutions you are submitting are too large to be submitted in this manor.  We are very sorry of the inconvience.  Please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
				}elseif( $_FILES['solutions']['error'] === UPLOAD_ERR_PARTIAL )
				{	$error .= "There was an error in the transmission of your solutions.  Please try again.  If the problem persists, please email your solutions to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
				}else
				{	// No solutions submitted
				}
			}else
			{	// No exam
				$error .= "You must include an exam with your submission<br />";
			}

			// Add exam to the database
			if( $error || !$database->addExam($exam) )
			{	// Failed to add exam.  Should output error
				$error .= "Database upload has failed<br />";
			}else
			{	$error .= "Your exam has been submitted without problems<br />";
				$success = true;
			}
		}
	}else
	{	// Exam upload has failed
	}
}

// Print exam entry form

if( $prefixes = $database->getPrefixes() )
{
	foreach($prefixes as $prefix)
	{
		if( $prefix == $_POST['course_prefix'] )
			$prefix_options .= "<option selected value='{$prefix}'>{$prefix}</option>";
		else
			$prefix_options .= "<option value='{$prefix}'>{$prefix}</option>";
	}
	$prefix_select = "<select name='course_prefix'>{$prefix_options}</select>";
}else
{	$prefix_select = "<input type='text' name='course_prefix' value='{$_POST['course_prefix']}' />";
}

foreach( $database->getTypes() as $type )
{	if( $type == $_POST['type'] )
		$exam_options .= "<option selected value='{$type}'>{$type}</option>";
	else
		$exam_options .= "<option value='{$type}'>{$type}</option>";
}

if( $_POST['practice'] )
	$is_practice = "checked='true' ";

// Print last 15 terms as options (no one should really have exams older than 5 years
$term = current_term();
for( $i = 0; $i < 28; $i++ )
{	
	$term_attr = number_to_term($term);
	if( $term == $_POST['term'] )
		$term_options .= "<option selected value='{$term}'>{$term_attr['year']} {$term_attr['term']}</option>";
	else
		$term_options .= "<option value='{$term}'>{$term_attr['year']} {$term_attr['term']}</option>";

	if( $term % 10 == 1 )
		$term -= 2;
	else
		$term -= 4; 
}

if( $success )
{	$colour = "blue";
}else
{	$colour = "red";
}

$template->setCurrentBlock("MAIN_CONTENT");
$template->setVariable("MAIN_HEADER", "Exambank: Submit Exam");
$template->setVariable("MAIN_BODY", "<p>Please ensure that you have permission share your exam before you post it.  If you have any problems with the submission form, you can email <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc</a> for help.</p>
<p style='color:{$colour}; font-weight:bold'>{$error}</p>
<form action='{$_SERVER['SCRIPT_NAME']}' enctype='multipart/form-data' method='post'>
<table>
	<tr><td>Course Prefix:</td><td>{$prefix_select}</td></tr>
	<tr><td>Course Number:</td><td><input type='text' name='course_number' size='4' value='{$_POST['course_number']}' /></td></tr>
	<tr><td>Term:</td><td><select name='term'>{$term_options}</select></td></tr>
	<tr><td>Exam Type:</td><td><select name='type'>{$exam_options}</select> #<input type='text' name='index' size='2' value='{$_POST['index']}'/></td></tr>
	<tr><td>Pratice:</td><td><input type='checkbox' name='practice' {$is_practice}/></td></tr>
	<tr><td>Exam:</td><td><input type='file' name='exam' size='20' value='{$_FILES['exam']['name']}' /></td></tr>
	<tr><td>Solutions:</td><td><input type='file' name='solutions' size='20' value='{$_FILES['solutions']['name']}'/></td></tr>
	<tr><td colspan='2' align='center'><input type='submit' name='submit' value='Submit' /></td></tr>
</table>
</form>
");
$template->parseCurrentBlock();

?>
