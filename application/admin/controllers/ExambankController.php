<?php

require_once 'MathSocAction.inc';
require_once 'examDB.inc';

class Admin_ExambankController extends MathSoc_Controller_Action
{
	protected $db;
	protected $examDir;

	public function init($secure = true)
	{	parent::init(false);

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
			$exam = $this->db->getUnapprovedExam( $this->_getParam($req) );
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

		// Parse request form to determine which exams to return
		$exams = $this->db->reviewExams( $_SESSION, $this->_getParam('page') );

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
