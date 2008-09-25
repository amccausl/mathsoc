<?php

require_once 'Zend/Controller/Action.php';

// Include module to display version differences
require_once 'diff.php';

class Council_PoliciesController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->initView();
		$this->db = new PolicyDB();
	}

/*
    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}
*/

	// Display an table of contents for the policies
	public function indexAction()
	{	
	}

	// Display a given policy
	public function displayAction()
	{
		$this->_getParam('page');
		// TODO: database lookup for current version of the policy
	}

	// Display a diff between two different versions of a policy
	public function diffAction()
	{	$version1 = $this->_getParam('version1');
		$version2 = $this->_getParam('version2');

		// TODO: database lookup for the content of each of the versions

		// TODO: add the diffs and other variables to the template
		$diff = new diff( $version1, $version2 );
		$this->view->diff = $diff->render();
	}
}
