<?php

require_once 'MathSocAction.inc';

class Admin_ClubsController extends MathSocAuth_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();
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
	{	// List the existing exams
	}
}
