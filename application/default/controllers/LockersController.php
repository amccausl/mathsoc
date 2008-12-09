<?php

require_once 'MathSocAction.inc';
require_once 'lockerDB.inc';

class LockersController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
		$this->view->username = Zend_Auth::getInstance()->getIdentity();
		$this->db = new LockerDB();
	}

	/**
	 * Checks to ensure that the system is active and displays information for the student's current locker
	 */
    public function indexAction()
    {
    }

	/** Displays a map the user can choose where they want their locker
	 * 
	 */
	public function mapAction()
	{
	}

	/** Displays a block of lockers and an imagemap
	 * 
	 */
	public function blockAction()
	{
	}

	/** Displays an image of a block of lockers or map
	 * 
	 */
	public function imageAction()
	{
	}

	/** Display a message explaining that the system is currently locked
	 * 
	 */
	public function lockedAction()
	{
	}

	/**
	 * Allow students to sign up for lockers
	 */
	public function signupAction()
	{
		require_once( "../application/default/views/helpers/form.inc" );

		// Initialize default form values
		$format = "%Y-%m-%d %H:%M:%S";
		$default = array(
			'locker_id' => $this->_getParam('locker_id'),
			'username' => Zend_Auth::getInstance()->getIdentity(),
			'email' => Zend_Auth::getInstance()->getIdentity() . '@uwaterloo.ca',
			'locker_current_phone' => '',
			'locker_combo' => '',
			'locker_expires' => strftime($format, locker_expires())
		);

		// Include form validation
		$smarty = $this->view->getEngine();
		if(empty($_POST))
		{	SmartyValidate::connect($smarty, true);
			SmartyValidate::register_validator('email_element','email','notEmpty');
		}else
		{	SmartyValidate::connect($smarty);
			// validate after a POST
			if(SmartyValidate::is_valid($_POST))
			{	// no errors, done with SmartyValidate
				//SmartyValidate::disconnect();

				if( $_POST['renew'] )
				{
				}elseif( $_POST['update'] )
				{
				}elseif( $_POST['change'] )
				{
				}

				// TODO: insert entry to database

				// TODO: forward to index
			}

			$this->view->assign($default);
			$this->view->assign($_POST);
        }

		}
	}
}

