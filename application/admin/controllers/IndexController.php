<?php

require_once 'MathSocAction.inc';

class Admin_IndexController extends MathSocAuth_Controller_Action
{
	private $db;

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
