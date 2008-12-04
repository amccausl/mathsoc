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

class Admin_RefundsController extends MathSocAuth_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();
		$this->db = new UserDB();
	}

	/** /admin/refunds/ - 
	 */
	public function indexAction()
	{	// Display current status of the system (term totals, recently rejected users)
		// TODO: form to add users or file of users
		// TODO: add drop down of all users
		// TODO: add links to other refunds actions
	}

	/** /admin/refunds/reject - allow the VPF to reject a user.
	 */
	public function rejectAction()
	{	// TODO: add form css & javascript

		//if( isset( $this->_getParam('userId') ) )
		//{
		//}

		// TODO: If the refunds period has closed, parse request logs against refund list, update rejected users
		// TODO: display dropdown for all 'REQUESTED' users requesting and reason text box
		//$this->view->select_options = $this->db->getRefunds('REQUESTED');
	}

	/** /admin/refunds/allow - allow the VPF to allow access to a user who was mistakenly denied
	 */
	public function allowAction()
	{	// TODO: add list of users from last term of status 'REGULAR'
		// TODO: add dropdown with all users whos status is REGULAR for the past term
		$allowed = $this->db->getRefunds('REGULAR');
		// TODO: add last term as a parameter to below
		$select_options = $this->db->getRefunds('RECEIVED');
		// TODO: if post set, update status of ID and term to 'REGULAR'
	}

	/** /admin/refunds/trend - display information for past terms and current progress
	 */
	public function trendAction()
	{	$trend = $this->db->getTrend();
		$return = array();
		foreach( $trend as $row )
		{	$return = array_merge( $return, array_values($row) );
		}
		
		$this->view->trend = $return;
	}
}
