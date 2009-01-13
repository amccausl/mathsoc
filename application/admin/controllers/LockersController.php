<?php

require_once 'MathSocAction.inc';
require_once 'lockerDB.inc';

class Admin_LockersController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		// User must be authenticated to see any of these pages
		// TODO: check to ensure that the user is office manager or exec

		$this->db = new LockerDB();
	}

	// Browsing Functions
	public function indexAction()
	{	// Call getUsage() and display results
		$usage = $this->db->getUsage();

		$this->view->assign( $usage );
	}

	public function lookupAction()
	{
		$lockers = $this->db->lookup();

		// Must dump associative array to single array
		$cols = "";
		$data = array();

		foreach( $lockers as $locker )
		{
			if( !$cols )
				$cols = implode(",", array_keys( $locker ));
			foreach( $locker as $col )
				array_push( $data, $col );
		}

		// Add results to view to create table
		$this->view->cols = $cols;
		$this->view->data = $data;
	}

	public function emptyAction()
	{	// Call getEmptyLockers() to expire lockers and return empty ones
		$lockers = $this->db->getEmptyLockers();

		// Must dump associative array to single array

		$cols = "";
		$data = array();

		foreach( $lockers as $locker )
		{
			if( !$cols )
				$cols = implode(",", array_keys( $locker ));
			foreach( $locker as $col )
				array_push( $data, $col );
		}

		// Add results to view to create table
		$this->view->cols = $cols;
		$this->view->data = $data;
	}
}
