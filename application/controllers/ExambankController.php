<?php

require_once 'Zend/Controller/Action.php';

class ExambankController extends Zend_Controller_Action
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
		print("hello world");
	}

	public function infoAction()
	{	// Flip with params
		print( "info action" );
		print_r( $_SERVER );
		
	}

	public function submitAction()
	{
	}

	// Javascript Callback Functions
	public function courseAction()
	{	// Used by ajax to list the courses with exams
	}

	public function examAction()
	{	// Used to retrieve exams in iframe
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
