<?php

require_once 'Zend/Controller/Action.php';

class CouncilController extends Zend_Controller_Action
{
    public function indexAction()
    {	// TODO: display summary page with 
    }

	public function representationAction()
	{	require_once( "userDB.inc" );
		
		// TODO: database call for current members
		// TODO: assign variables to template
	}

	public function positionAction()
	{	$position = $this->_getParam('position');

		// TODO: load position information from model

		// Display position name, description, comitement, current holder, and application information
	}

	public function minutesAction()
	{	// TODO: load wiki minutes module
	}

	public function pinkbookAction()
	{	// TODO: load wiki policies and bylaws module
	}

	public function vacanciesAction()
	{	// TODO: display available positions, requirements, responsibilities, and application information
	}
}

