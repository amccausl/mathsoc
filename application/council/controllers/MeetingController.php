<?php

require_once 'Zend/Controller/Action.php';

/**
 * This page should contain some general information about meetings and some Speaker and Secretary convience functions.
 */

class Council_MeetingController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();
		$this->initView();
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

	public function indexAction()
	{
	}

	/*
	 * Display information about a given meeting.  This includes the agenda (linked to policies/bylaws, attachments discussed), minutes
	 */
	public function infoAction()
	{
	}

	/*
	 * Allow the Speaker or President to call a meeting.  Post notice on webpage, calendar, mailing list.
	 */
	public function callAction()
	{
	}

	/*
	 * Allow for editting the agenda or location of a meeting
	 */
	public function editAction()
	{
	}

	public function minutesAction()
	{
		// TODO: extract params, call wikiPage
		// Create skeleton 
	}
}
