<?php

require_once 'MathSocAction.inc';

// Load the database model for the exam storage
require_once 'examDB.inc';

class Exambank_IndexController extends MathSoc_Controller_Action
{
	private $db;
	private $examDir;

	public function init()
	{	parent::init();
		// User must be authenticated to see any of these pages
		$this->secure();

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
			$exam = $this->db->getApprovedExam( $this->_getParam('id') );

			// Ensure the type of exam we're looking for exists
			if( !$filename = $exam[$this->_getParam('type') . "_path"] )
			{	print( "The exam you're looking for doesn't exist" );
				exit;
			}
			$filename = $this->examDir . $filename;

			// Display the exam to the user
			if( $buffer = file_get_contents( $filename ) )
			{
				// Grab the file extension from the mime type
				$ext = split('/',$exam[$this->_getParam('type') . '_type']);
				$ext = $ext[1];

				// Output file information to browser
				header("Content-type: {$exam[$this->_getParam('type') . '_type']}");
				header("Content-Length: ".strlen($buffer));
				header("Content-Disposition: inline; filename={$exam['course']}-{$exam['term']}-{$exam['type']}{$exam['number']}_{$this->_getParam('type')}.{$ext}");

				echo($buffer);
				exit;
			}
		}elseif( $this->_getParam('prefix') && $this->_getParam('number') )
		{	// Load the "exams" array to the view
			// Must set exam.id, exam.course, exam.term, exam.file_path, exam.sol_path
			$params = array(
						'course_prefix'	=> $this->_getParam('prefix'),
						'course_code'	=> $this->_getParam('number'),
						'status'		=> 'approved'
						);
			$exams = $this->db->getExams( $params );
			$this->view->exams = $exams;
		}
	}

	public function submitAction()
	{
		$menu = $this->view->menu;
		$menu[1]['status'] = "active";
		$menu[1]['sub'][0]['status'] = "active";
		$menu[1]['sub'][0]['sub'][5]['status'] = "active selected";
		$this->view->menu = $menu;
	
		require_once( "../application/default/views/helpers/form.inc" );
		$this->view->array_push( 'stylesheets', $this->getRequest()->getBaseUrl() . '/css/form.css' );

		// Generate HTML elements for the form
		$prefixes = $this->db->getCourses(null, null, true);
		$this->view->prefix_options = array_combine( $prefixes, $prefixes );

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

		$types = $this->db->getTypes();
		$this->view->type_options = array_combine( $types, $types );

		if( isset( $_POST['submit'] ) )
		{
			$error = "";
			$success = false;

			// Create exam object to add
			$exam = array();
			$exam['uploader'] = Zend_Auth::getInstance()->getIdentity();

			// Must validate all form info and translate to $exam

			// Check prefix exists and is valid
			if( !isset( $_POST['course_prefix'] ) )
			{	// Prefix has not been set, but is manditory
				$error .= "You must enter the prefix for the course.<br />";
			}else
			{	$course = $this->db->getCourses( strtoupper( $_POST['course_prefix'] ), $_POST['course_code'], true );
				if( !$course )
				{	// The course doesn't exist in the database
					$error .= "The course you have selected was not found in the database.<br />";
				}else
				{	$exam['courseId'] = $course['id'];
				}
			}

			$exam['term'] = $_POST['course_term'];

			// Check if type exists and is valid
			if( !isset( $_POST['exam_type'] ) )
			{	$error .= "You must select the type of the exam<br />";
			}else
			{	$exam['type'] = $_POST['exam_type'];
			}

			// Add index if valid
			if( ereg( "^[0-9]+$", $_POST['index'] ) )
			{	$exam['index'] = $_POST['index'];
			}

			// Check status of practice checkbox
			if( isset($_POST['practice']) )
				$exam['practice'] = 1;		

			if( !$error )
			{
				$exam_file = file_upload( 'exam_file', $this->examDir );
				switch( $exam_file[0] )
				{
					case UPLOAD_ERR_OK:
						$exam['exam_path'] = $exam_file[1];
						$exam['exam_type'] = $_FILES['exam_file']['tmp_name'];
						break;
					case UPLOAD_ERR_INI_SIZE:
					case UPLOAD_ERR_FORM_SIZE:
						$error .= "The exam you are submitting is too large to be submitted in this manor.  We are very sorry of the inconvience.  Please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
						break;
					case UPLOAD_ERR_PARTIAL:
						$error .= "There was an error in the transmission of your exam.  Please try again.  If the problem persists, please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
						break;
				}

				$solutions_file = file_upload( 'solutions_file', $this->examDir );
				switch( $solutions_file[0] )
				{
					case UPLOAD_ERR_OK:
						$exam['solutions_path'] = $solutions_file[1];
						$exam['solutions_type'] = $_FILES['solutions_file']['tmp_name'];
						break;
					case UPLOAD_ERR_INI_SIZE:
					case UPLOAD_ERR_FORM_SIZE:
						$error .= "The solutions you are submitting are too large to be submitted in this manor.  We are very sorry of the inconvience.  Please email your exam to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
						break;
					case UPLOAD_ERR_PARTIAL:
						$error .= "There was an error in the transmission of your solutions.  Please try again.  If the problem persists, please email your solutions to <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc.uwaterloo.ca</a><br />";
						break;
				}

				if( !isset( $exam['exam_path'] ) && !isset( $exam['solutions_path'] ) )
					$error .= "You must include an exam with your submission<br />";

				// Add exam to the database
				if( $error || !$this->db->addExam($exam) )
				{	// Failed to add exam.  Should output error
					$error .= "Database upload has failed<br />";
				}else
				{	$error .= "Your exam has been submitted without problems<br />";
					$success = true;
				}
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
