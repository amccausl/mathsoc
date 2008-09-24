<?php

require_once 'Zend/Controller/Action.php';

class CouncilController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();
		$this->initView();
	}

    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}

	public function indexAction()
	{	// Display some information about council and a current list of councillors
	}

	public function policiesAction()
	{	// Display current policies wiki
	}

	public function minutesAction()
	{	// Display minutes wiki
	}

	public function adminAction()
	{	
	}

//TODO: consider moving elections material to another controller
	public function electionsAction()
	{	
	}

	public function votingAction()
	{	// Allow users to vote in available elections
	}

	public function representationAction()
	{	
	}

	public function vacanciesAction()
	{	// Display the currently vacant positions
	}
}
