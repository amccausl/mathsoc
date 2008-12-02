<?php

require_once 'MathSocAction.inc';

// Load the database model for CIF information storage
require_once 'cifDB.inc';

class Admin_CifController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new CifDB();
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

	/** /admin/cif/ - Display current status of system and list proposals
	 *
	 */
	public function indexAction()
	{	// TODO: database lookup for current proposals
	}
}
