<?php

require_once 'MathSocAction.inc';
require_once 'examDB.inc';

class Admin_ExambankController extends MathSoc_Controller_Action
{
	protected $db;
	protected $examDir;

	public function init($secure = true)
	{	parent::init($secure);

		$config = new Zend_Config_Ini('../config/main.ini', 'exambank');
		$this->examDir = $config->examDir;

		$this->db = new ExamDB();
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
		// TODO: include summary of system use, number of exams, number of uses this term and which users, link to review, list of courses
	}

	// Admin Functions
	public function reviewAction()
	{	// For admin to review submitted exams

		// If an exam is requested, return it to the user.
		if( $this->_getParam('exam') || $this->_getParam('solutions') )
		{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);

			$req = $this->_getParam('exam') ? 'exam' : 'solutions';
			$exam = $this->db->getUnapprovedExam( $this->getParam($req) );
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

		// Display all unapproved exams to the user
		$exams = $this->db->getUnapprovedExams();
		$this->view->exams = $exams;
	}

	public function updateAction()
	{	// Update information for an exam pending approval
		// TODO: display and process form to update the information on an exam
		// TODO: relink to review
	}
}
