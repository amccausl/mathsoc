<?php

require_once 'MathSocAction.inc';
require_once 'userDB.inc';

/**
 * Should provide methods to add list of refundees, review people who used
 * the online systems anyway, reject refunds, retrieve lists, and mark a user 
 * as paid for the term.  It would also be neat to include trending (number
 * of students each term and track the success of each system).
 *
 */

class Admin_RefundsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new UserDB();

		$this->admins = array( 'mathsoc' => array( 'vpf' => 'current' ) );
		$this->secure();
	}

	/** /admin/refunds/ - 
	 */
	public function indexAction()
	{	// Display current status of the system (term totals, recently rejected users)
		// TODO: form to add users or file of users
		// TODO: add drop down of all users
		// TODO: add links to other refunds actions
		// TODO: include bulk add of users
	}

	/** /admin/refunds/reject - allow the VPF to reject a user.
	 */
	public function rejectAction()
	{	require_once( 'form.inc' );

		if( $this->_getParam('user') && $this->_getParam('user_reason') )
		{	// TODO: validate and write entry to database
			// TODO: add success message to view
			if( $this->db->rejectRefund( $this->_getParam('user'), $this->_getParam('user_reason') ) )
			{
			}else
			{
			}
		}

		// TODO: If the refunds period has closed, parse request logs against refund list, update rejected users
		$this->view->user_options = $this->db->getRefunds('REQUESTED');
	}

	/** /admin/refunds/allow - allow the VPF to allow access to a user who was mistakenly denied
	 */
	public function allowAction()
	{	require_once( 'form.inc' );

		if( $this->_getParam('user') )
		{	if( $this->db->allowRefund($this->_getParam('user')) )
			{
			}else
			{
			}
		}

		// Calculate the number for the past term
		$term = (date('Y') - 1900) * 10 + floor((date('m') - 1) / 4) * 4 + 1;
		if( $term % 10 == 1 )
		{	$term -= 2;
		}else
		{	$term -= 4;
		}

		// Add list of people who already have been enabled
		$this->view->user_allowed = implode( ", ", $this->db->getRefunds('REGULAR', $term));

		// Add list of users who got their refunds last term
		$this->view->user_options = $this->db->getRefunds('RECEIVED', $term);
	}

	/** /admin/refunds/trend - display information for past terms and current progress
	 */
	public function trendAction()
	{	$result = $this->db->getTrend();
		$trend = array();
		foreach( $result as $row )
		{	$trend = array_merge( $trend, array_values($row) );
		}
		
		$this->view->trend = $trend;
	}
}
