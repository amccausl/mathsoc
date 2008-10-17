<?php

require_once 'Zend/Controller/Action.php';

class LockersController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->username = Zend_Auth::getInstance()->getIdentity();
		$this->db = new LockerDB();
	}

	/**
	 * Checks to ensure that the system is active and displays information for the student's current locker
	 */
    public function indexAction()
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
			//SmartyValidate::register_validator('prefix_element','course_prefix','notEmpty');
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

	/**
	 * This displays information on all expiring lockers to the office manager.
	 */
	public function adminAction()
	{
	}
}

