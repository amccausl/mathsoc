<?php

require_once 'MathSocAction.inc';

// Load the database model for the exam storage
require_once 'examDB.inc';

class Exambank_IndexController extends MathSoc_Controller_Action
{
	private $db;
	private $examDir;

	public function init($secure = true)
	{	parent::init($secure);
		// User must be authenticated to see any of these pages

		$config = new Zend_Config_Ini('../config/main.ini', 'exambank');
		$this->examDir = $config->examDir;

		$this->db = new ExamDB();
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams

		// Add the javascripts to layout for DHTML interface
		$this->view->javascripts = array(
			$this->getRequest()->getBaseUrl() . '/js/prototype.js',
			$this->getRequest()->getBaseUrl() . '/js/course_selector.js');
	}

	// Javascript Callback Functions
	public function coursesAction()
	{	// Used by ajax to list the courses with exams
		if( $this->_getParam('number') )
		{	// Display course title information
			$this->view->values = $this->db->getCourses($this->_getParam('prefix'), $this->_getParam('number'));
		}elseif( $this->_getParam('prefix') )
		{	// Display course numbers for prefix
			$this->view->values = $this->db->getCourses($this->_getParam('prefix'));
		}else
		{	// Display all the prefixes available
			$this->view->values = $this->db->getCourses();
		}
	}

	/** /exambank/exams/:prefix/:number or /exambank/exams/:prefix/:number/:term/:id/:type/
	 *
	 */
	public function examsAction()
	{	// Used to retrieve exams search results in iframe
		if( $this->_getParam('type') )
		{	// Disable view renderer so output can be created manually
			Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);

			// Lookup exam in system
			$exam = $this->db->getExam( $this->_getParam('id') );

			// Ensure the type of exam we're looking for exists
			if( !$filename = $exam[$this->_getParam('type') . "_path"] )
			{	print( "The exam you're looking for doesn't exist" );
				exit;
			}
			$filename = $this->examDir . $filename;

			// Display the exam to the user
			if( $buffer = file_get_contents( $filename ) )
			{
				header("Content-type: {$exam[$this->_getParam('type') . '_type']}");

				// Grab the file extension from the mime type
				$ext = split('/',$exam[$this->_getParam('type') . '_type']);
				$ext = $ext[1];

				//header("Content-Length: ".strlen($buffer));
				//header("Content-Disposition: inline; filename={$exam['course']}-{$exam['term']}-{$exam['type']}{$exam['number']}_{$this->_getParam('type')}.{$ext}");

				readfile($filename);
				exit;
			}
		}elseif( $this->_getParam('prefix') && $this->_getParam('number') )
		{	// Load the "exams" array to the view
			// Must set exam.id, exam.course, exam.term, exam.file_path, exam.sol_path
			$this->view->exams = $this->db->getExams( $this->_getParam('prefix'), $this->_getParam('number') );
		}
	}

	public function submitAction()
	{
		require_once( "../application/default/views/helpers/form.inc" );

		// Generate HTML elements for the form
		$this->view->prefix_options = $this->db->getCourses(null, null, true);

		$terms = array();
		$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
		$seasons = array( "W", "S", "F" );
		for( $i = 0; $i < 15; $i++ )
		{ 
			$terms[$term] = $seasons[floor($term%10/4)] . " " . (floor($term/10) + 1900);

			if( $term % 10 == 1 )
				$term -= 2;
			else
				$term -= 4;
		}
		$this->view->term_options = $terms;

		if( isset( $_POST['submit'] ) )
		{
			// Create exam object to add
			$exam = array();
			$exam['uploader'] = Zend_Auth::getInstance()->getIdentity();

			// Must validate all form info and translate to $exam

			// Check prefix exists and is valid
			if( !isset( $_POST['course_prefix'] ) )
			{	// Prefix has not been set, but is manditory
				$error .= "You must enter the prefix for the course.<br />";
			}elseif ( !isset( $_POST['course_prefix'] ) )
			{	// Prefix is not a valid prefix
				$error .= "You must include a valid prefix<br />";
			}else
			{	//if( !in_array( strtoupper( $_POST['course_prefix'] ), $database->getPrefixes() ) )
				//{	// Given prefix is unknown
				//	$error .= "The prefix you have chosen is unknown.<br />";
				//}else
				//{	// Check number exists and is valid
				//	if( !isset( $_POST['course_number'] ) )
				//	{	// Course number has not been included
				//		$error .= "You must enter the number of the course.<br />";
				//	}elseif( !sanitize( $_POST['course_number'], "course_number" ) )
				//	{	// Course number included is not a course number
				//		$error .= "You must include a valid course number.<br />";
				//	}else
				//	{	// Ensure the course exists
					if( !$course = $database->getCourses( strtoupper( $_POST['course_prefix'] ), $_POST['course_number'], true ) )
					{	// The course doesn't exist in the database
						$error .= "The course you have selected was not found in the database.<br />";
					}else
					{	$exam['courseId'] = $course['id'];
					}
				//	}
				//}
			}

			$exam['term'] = $_POST['term'];

			// Check if type exists and is valid
			if( !isset( $_POST['type'] ) )
			{	$error .= "You must select the type of the exam<br />";
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
			
							if( move_uploaded_file( $_FILES['exam']['tmp_name'], $this->examDir . $uploadExam ) )
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

							if( move_uploaded_file( $_FILES['solutions']['tmp_name'], $this->examDir . $uploadSol ) )
							{	$exam['sol_path'] = $uploadSol;
								$exam['sol_type'] = $_FILES['solutions']['type'];
								chmod( $this->examDir . $uploadSol, 0575 );
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

			if( $success )
			{	$this->view->colour = "blue";
			}else
			{	$this->view->colour = "red";
			}

			$this->view->error = $error;
		}

		$this->view->assign($_POST);
	}
}
