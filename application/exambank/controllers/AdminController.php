<?php

require_once 'MathSocAction.inc';

class Exambank_AdminController extends MathSoc_Controller_Action
{
	// TODO: add authentication and authorization for all these pages

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
	}

	// Admin Functions
	public function reviewAction()
	{	// For admin to review submitted exams
	}

	public function approveAction()
	{	// Used to approve submitted exam
	}

	public function removeAction()
	{	// Used to remove an exam that was pending approval
	}

	public function updateAction()
	{	// Update information for an exam pending approval
	}
}
