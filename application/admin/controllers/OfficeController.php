<?php

require_once 'MathSocAction.inc';

// Load the database model for the exam storage
require_once '../application/exambank/models/examDB.inc';

class Admin_OfficeController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new ExamDB();
	}
/*
    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}
*/
	// Browsing Functions
	public function indexAction()
	{
	}

	/** Retrieve a list of the email address of the office workers for the term
	 */
	public function emailAction()
	{
	}
}
