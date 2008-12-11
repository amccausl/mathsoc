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
	{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);

		$start = $_GET['start'];	// First locker in a block
		$end = $_GET['end'];		// Last locker in a block
		$height = 6; 				// Height of locker bank
		$width = ceil( ( $end - $start ) / $height );

		$filled = $database->taken( $start, $end );

		// create the image
		$img_handle = ImageCreate(($width)*50 + 30, ($height)*50 + 30) or die ("Cannot Create image"); 
		$white = imagecolorallocate($img_handle, 255,255,255);	// First color is the background color
		$black = imagecolorallocate($img_handle, 0,0,0);	// Make black and red
		$red = imagecolorallocate($img_handle, 255,0,0);

		// Draw the lockers and fill them.
		$count = $start-1;
		for ($x=0;$x <= $width-1; $x++)
		{
			for ($y=0;$y <= $height-1; $y++)
			{
				$count++;

				// Draw the outlines
				imagerectangle($img_handle, $x*50,$y*50,($x+1)*50,($y+1)*50, $black);
				// Fill them red if they are signed out.
				if( $filled[$count] )
				{	imagefilledrectangle($img_handle, $x*50+1,$y*50+1,($x+1)*50-1,($y+1)*50-1, $red);
				}
				ImageString ($img_handle, 31, $x*50 + 15, $y*50 + 15, ($x*$height + $y + $start), $black);
			}
		}

		// Output image
		header ("Content-type: image/png");
		ImagePng( $img_handle );
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

