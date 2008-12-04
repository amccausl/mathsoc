<?php

require_once 'MathSocAction.inc';
require_once 'examDB.inc';

class Admin_ExambankController extends MathSoc_Controller_Action
{
	// TODO: add authentication and authorization for all these pages

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
		// TODO: include pager and links for all exams (maybe use javascript to browse nicely)
		// TODO: include links to remove or update each
	}

	// Admin Functions
	public function reviewAction()
	{	// For admin to review submitted exams

		// TODO: lookup unapproved exams from database
		// TODO: include links to remove, approve, or update
	}

	public function approveAction()
	{	// Used to approve submitted exam
	}

	public function removeAction()
	{	// Used to remove an exam that was pending approval

		//
	}

	public function updateAction()
	{	// Update information for an exam pending approval
		// TODO: display and process form to update the information on an exam
	}
}
