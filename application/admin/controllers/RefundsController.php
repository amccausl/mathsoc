<?php

require_once 'MathSocAction.inc';

/**
 * Should provide methods to add list of refundees, review people who used
 * the online systems anyway, reject refunds, retrieve lists, and mark a user 
 * as paid for the term.  It would also be neat to include trending (number
 * of students each term and track the success of each system).
 *
 * TODO: alter database to store rejections and reasoning
 * TODO: add trending
 */

class Admin_RefundsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();
	}

    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}

	/** /admin/refunds/ - 
	 */
	public function indexAction()
	{
	}

	/** /admin/refunds/reject - allow the VPF to reject a user.
	 */
	public function rejectAction()
	{
	}

	/** /admin/refunds/allow - allow the VPF to allow access to a user who was mistakenly denied
	 */
	public function allowAction()
	{
	}

	/** /admin/refunds/trend - display information for past terms and current progress
	 */
	public function trendAction()
	{
	}
}
