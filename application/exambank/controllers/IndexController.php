<?php

require_once 'Zend/Controller/Action.php';

class Exambank_IndexController extends Zend_Controller_Action
{
	// Common variables to Exambank system

	// Common Initialization
	public function init()
	{
		parent::init();
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
	}

	// Javascript Callback Functions
	public function courseAction()
	{	// Used by ajax to list the courses with exams
	}

	public function examsAction()
	{	// Used to retrieve exams search results in iframe
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
