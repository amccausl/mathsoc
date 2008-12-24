<?php

require_once 'MathSocAction.inc';

class Admin_UserController extends MathSocAuth_Controller_Action
{
	// The database object to retrieve information from
	private $db;

	// The groups that are an admin of this system
	private $admins = array();

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();

	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
	}
}
