<?php

require_once 'MathSocAction.inc';

require_once 'announceDB.inc';

class Admin_EventsController extends MathSocAuth_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

	}

	/** /admin/events/ - Display current status of system and list proposals
	 *
	 */
	public function indexAction()
	{	
	}

	/** /admin/event/add/ - Display form to create new events.
	 *
	 */
	public function addAction()
	{	// TODO: add event to google calendars	
	}

	/** /admin/events/signup/ - Display a table of signup-able events, or the people who have signed up for one.
	 * Display events, allow users to enable signup for events, or select events and see users.
	 */
	public function signupAction()
	{
	}
}
