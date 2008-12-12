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
		switch( $_GET['block'] )
		{	case 1:		$start = 1;		$end = 48;	break;
			case 2:		$start = 49;	$end = 96;	break;
			case 3:		$start = 97;	$end = 186;	break;
			case 4:		$start = 187;	$end = 198;	break;
			case 5:		$start = 199;	$end = 270;	break;
			case 6:		$start = 271;	$end = 354;	break;
			case 7:		$start = 355;	$end = 414;	break;
			case 8:		$start = 415;	$end = 492;	break;
			case 9:		$start = 493;	$end = 570;	break;
			case 10:	$start = 571;	$end = 594;	break;
			case 11:	$start = 595;	$end = 668;	break;
			case 12:	$start = 669;	$end = 732;	break;
			case 13:	$start = 733;	$end = 786;	break;
		}

		// Set block settings in the view
		$this->view->start = $start;
		$this->view->end = $end;

		// Allocate and define dimensions for the block of lockers
		$lockers = array();
		$height = 6;				// Height of locker bank
		$width = ceil( ( $end - $start ) / $height );
		for ($y=0;$y <= $height-1; $y++)
		{	for ($x=0;$x <= $width-1; $x++)
			{	$lockers[$start + $x*$height + $y] = array($x*50,$y*50,($x+1)*50,($y+1)*50);
			}
		}

		$this->view->height = $height;
		$this->view->width = $width;
		$this->view->lockers = $lockers;

		// Output image of block if the format is set
		if( $this->_getParam('format') )
		{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);

			//$filled = $this->db->taken( $start, $end );

			// create the image
			$img_handle = ImageCreate(($width)*50 + 30, ($height)*50 + 30) or die ("Cannot Create image"); 
			$white = imagecolorallocate($img_handle, 255,255,255);	// First color is the background color
			$black = imagecolorallocate($img_handle, 0,0,0);	// Make black and red
			$red = imagecolorallocate($img_handle, 255,0,0);

			foreach( $lockers as $number => $coords )
			{
				imagerectangle($img_handle, $coords[0], $coords[1], $coords[2], $coords[3], $black);
				// Fill them red if they are signed out.
				if( $filled[$count] )
				{	imagefilledrectangle($img_handle, $coords[0]+1,$coords[1]+1,$coords[2]-1,$coords[3]-1, $red);
				}
				ImageString ($img_handle, 31, $coords[0] + 15, $coords[1] + 15, $number, $black);
			}

			// Output image
			header ("Content-type: image/png");
			ImagePng( $img_handle );
		}else
		{	// Otherwise, output image map
		}
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

