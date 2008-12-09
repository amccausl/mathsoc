<?php

require_once 'MathSocAction.inc';

// Load the database model for the exam storage
require_once '../application/models/electionsDB.inc';

class Admin_ElectionsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new ExamDB();
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
	}
}
