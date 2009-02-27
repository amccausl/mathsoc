<?php

require_once 'MathSocAction.inc';
require_once 'examDB.inc';

class Admin_ExambankController extends MathSoc_Controller_Action
{
	protected $db;
	protected $examDir;

	public function init()
	{	parent::init();

		$config = new Zend_Config_Ini('../config/main.ini', 'exambank');
		$this->examDir = $config->examDir;

		$this->db = new ExamDB();
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams

		// If an exam is requested, return it to the user.
		if( $this->_getParam('exam') || $this->_getParam('solutions') )
		{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);

			$req = $this->_getParam('exam') ? 'exam' : 'solutions';
			$exam = $this->db->getExam( $this->_getParam($req) );
			if( !$filename = $exam[$req . '_path'] )
			{	echo( 'The exam you\'re looking for doesn\'t exist' );
				exit;
			}
			$filename = $this->examDir . $filename;

			// Output the file to the user
			if( file_exists( $filename ) )
			{	header('Content-type: ' . $exam[$req . '_type']);
				readfile($filename);
				exit;
			}else
			{	echo( 'The exam file is missing.' );
				exit;
			}
		}

		// Process the search form
		$filter = isset( $_SESSION['exam_filter'] ) ? $_SESSION['exam_filter'] : array();

		$prefixes = $this->db->getCourses(null, null);
		array_unshift( $prefixes, 'ALL' );
		$this->view->prefix_options = array_combine( $prefixes, $prefixes );
		if( isset( $_POST['course_prefix'] ) && $_POST['course_prefix'] != 'ALL')
		{	$filter['course_prefix'] = $_POST['course_prefix'];
		}else
		{	unset( $filter['course_prefix'] );
		}

		if( isset( $_POST['course_code'] ) && $_POST['course_code'] )
		{	$filter['course_code'] = $_POST['course_code'];
		}else
		{	unset( $filter['course_code'] );
		}

		$terms = array('ALL' => 'ALL');
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
		if( isset( $_POST['term'] ) && $_POST['term'] != 'ALL' )
		{	$filter['term'] = $_POST['term'];
		}else
		{	unset( $filter['term'] );
		}

		$status_options = array( 'all', 'pending', 'approved', 'rejected' );
		$this->view->status_options = array_combine( $status_options, $status_options );
		if( isset( $_POST['status'] ) && $_POST['status'] != 'all' )
		{	$filter['status'] = $_POST['status'];
		}else
		{	unset( $filter['status'] );
		}

		$exams = $this->db->getExams( $filter, $this->_getParam('page') );
		$this->view->assign($filter);
		$_SESSION['exam_filter'] = $filter;

		// Process user input if necessary
		$c = count( $exams );
		for( $i = 0; $i < $c; $i++ )
		{	if( isset( $_POST['approve_' . $exams[$i]['id']] ) )
			{	if( $this->db->approveExam( $exams[$i]['id'] ) )
					$exams[$i]['status'] = 'approved';
				else
					$this->view->error = 'There was a problem approving the exam.  Please try again later.';
				
			}elseif( isset( $_POST['reject_' . $exams[$i]['id']] ) )
			{	if( $this->db->rejectExam( $exams[$i]['id'] ) )
					$exams[$i]['status'] = 'rejected';
				else
					$this->view->error = 'There was a problem rejecting the exam.  Please try again later.';
			}elseif( isset( $_POST['update_' . $exams[$i]['id']] ) )
			{	$this->_redirect( '/admin/exambank/update/id/' . $exams[$i]['id'] );
			}else
			{
			}
		}

		// Display all unapproved exams to the user
		$this->view->exams = $exams;
	}

	public function updateAction()
	{	// Update information for an exam pending approval
		// TODO: display and process form to update the information on an exam
		// TODO: relink to review
	}
}
