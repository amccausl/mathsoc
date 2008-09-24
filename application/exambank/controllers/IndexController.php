<?php

require_once 'Zend/Controller/Action.php';

// Load the database model for the exam storage
require_once '../application/exambank/models/examDB.inc';

class Exambank_IndexController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new ExamDB();
	}

    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams

		// Add the javascripts for DHTML interface
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

	public function examsAction()
	{	// Used to retrieve exams search results in iframe
		if( $this->_getParam('type') )
		{	// TODO: output file
			$exam = $this->db->getExam( $this->_getParam('id') );
		}elseif( $this->_getParam('prefix') && $this->_getParam('number') )
		{	// Load the "exams" array to the view
			// Must set exam.id, exam.course, exam.term, exam.file_path, exam.sol_path
			$this->view->exams = $this->db->getExams( $this->_getParam('prefix'), $this->_getParam('number') );
		}
	}
}
