<?php

require_once 'Zend/Controller/Action.php';

// Load the database model for the election system
require_once '../application/default/models/electionDB.inc';

class ElectionController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new ElectionDB();

		//$this->view->javascripts = array(
		//	$this->getRequest()->getBaseUrl() . "/js/prototype");

	}

    function preDispatch()
	{
		// User must be authenticated to use this system
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}

	// Browsing Functions
	public function indexAction()
	{	// Present the existing elections

	}

	// Cast a ballot in an election
	public function castAction()
	{
		if( !$auth->hasIdentity() )
		{
		}

		// Ensure that the desired fields are set
		$electionId = substr( $this->_getParam('electionId'), 8 );

		// Determine how the user voted
		$vote = array();
		for( $key = 0; isset( $_POST["c$key"] ); $key++ )
		{   array_push( $vote, $_POST["c$key"] );
		}

		// Add the users vote to the system
		if( $this->db->vote( $electionId, $_SESSION['username'], $vote ) )
		{   print( "( success : true )" );
		}else
		{   print( "( success : false )" );
		}
	}

	// Declare an election
	public function declareAction()
	{
	}

	// tally the ballots in a given election, display the results
	public function tallyAction()
	{
	}
}
