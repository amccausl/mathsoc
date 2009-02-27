<?php

require_once 'MathSocAction.inc';

// Load the database model for the election system
require_once 'electionDB.inc';

class ElectionController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// Initialize the elections database
		$this->db = new ElectionDB();

		// Add elections javascripts
		$this->view->array_push("javascripts", $this->getRequest()->getBaseUrl() . "/js/prototype" );
	}

	// Browsing Functions
	public function indexAction()
	{	// Grab the authenticated userid
		$user = Zend_Auth::getInstance()->getIdentity();
		// Present the existing elections
		$this->view->elections = $this->db->getElections( $user );
	}

	// Cast a ballot in an election
	public function castAction()
	{
		// Ensure that the desired fields are set
		$electionId = substr( $this->_getParam('electionID'), 8 );

		$user = Zend_Auth::getInstance()->getIdentity();

		// Determine how the user voted
		$vote = array();
		for( $key = 0; isset( $_POST["c$key"] ); $key++ )
		{   array_push( $vote, $_POST["c$key"] );
		}

		// Add the users vote to the system, display result to user
		$this->view->success = $this->db->vote( $electionId, $user, $vote );
	}
}
