<?php

require_once 'MathSocAction.inc';
require_once 'lockerDB.inc';

//class LockersController extends MathSocAuth_Controller_Action
class LockersController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init(true);

		$this->db = new LockerDB();

		$menu = $this->view->menu;

		for( $i = 0; $i < count( $menu ); $i++ )
		{	if( $menu[$i]['title'] == 'Services' )
			{	$menu[$i]['status'] = 'active';
				for( $j = 0; $j < count( $menu[$i]['sub'] ); $j++ )
				{	if( $menu[$i]['sub'][$j]['title'] == 'Lockers' )
					{	$menu[$i]['sub'][$j]['status'] = 'active selected';
					}
				}
			}
		}

		$this->view->menu = $menu;
	}

	/**
	 * Checks to ensure that the system is active and displays information for the student's current locker
	 */
    public function indexAction()
    {	
    }

	/** Displays a map the user can choose where they want their locker
	 * This action is static in the template file.
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
			case "3a":	$start = 97;	$end = 144;	break;
			case "3b":	$start = 145;	$end = 186;	break;
			case 4:		$start = 187;	$end = 198;	break;
			case "5a":	$start = 199;	$end = 234;	break;
			case "5b":	$start = 235;	$end = 270;	break;
			case "6a":	$start = 271;	$end = 307;	break;
			case "6b":	$start = 308;	$end = 354;	break;
			case "7a":	$start = 355;	$end = 384;	break;
			case "7b":	$start = 385;	$end = 414;	break;
			case "8a":	$start = 415;	$end = 452;	break;
			case "8b":	$start = 453;	$end = 492;	break;
			case "9a":	$start = 493;	$end = 530;	break;
			case "9b":	$start = 531;	$end = 570;	break;
			case 10:	$start = 571;	$end = 594;	break;
			case "11a":	$start = 595;	$end = 631;	break;
			case "11b":	$start = 632;	$end = 668;	break;
			case "12a":	$start = 669;	$end = 700;	break;
			case "12b":	$start = 701;	$end = 732;	break;
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

			$filled = $this->db->taken( $start, $end );

			// create the image
			$img_handle = ImageCreate(($width)*50 + 30, ($height)*50 + 30) or die ("Cannot Create image"); 
			$white = imagecolorallocate($img_handle, 255,255,255);	// First color is the background color
			$black = imagecolorallocate($img_handle, 0,0,0);	// Make black and red
			$red = imagecolorallocate($img_handle, 255,0,0);

			foreach( $lockers as $number => $coords )
			{
				imagerectangle($img_handle, $coords[0], $coords[1], $coords[2], $coords[3], $black);
				// Fill them red if they are signed out.
				if( $filled[$number] )
				{	imagefilledrectangle($img_handle, $coords[0]+1,$coords[1]+1,$coords[2]-1,$coords[3]-1, $red);
				}
				// Write the locker number in the center of each box
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
	{	// If locks are being cut, don't allow login
		if( !$this->db->isActive() )
		{	$this->_redirect("/Lockers/locked");
		}

		$locker = $this->db->lookup( Zend_Auth::getInstance()->getIdentity() );
		require_once( "../application/default/views/helpers/form.inc" );

		if( $locker )
		{	$id = $this->_getParam('locker_id') ? $this->_getParam('locker_id') : $locker['id'];
			$phone = $locker['current_phone'];
			$combo = $locker['combo'];
			$expires = $locker['expires'];

			$this->view->current_locker = $locker['id'];
		}else
		{	$id = $this->_getParam('locker_id');
			$phone = "";
			$combo = "";
			$expires = strftime($format, locker_expires());
		}

		// Initialize default form values
		$format = "%Y-%m-%d %H:%M:%S";
		$default = array(
			'locker_id' => $id,
			'username' => Zend_Auth::getInstance()->getIdentity(),
			'email' => Zend_Auth::getInstance()->getIdentity() . '@uwaterloo.ca',
			'locker_current_phone' => $phone,
			'locker_combo' => $combo,
			'expires' => $expires
		);
		$this->view->assign($default);

		// Include form validation
		$smarty = $this->view->getEngine();
		//if(empty($_POST))
		//{	SmartyValidate::connect($smarty, true);
		//	SmartyValidate::register_validator('email_element','email','notEmpty');
		//}else
		//{	SmartyValidate::connect($smarty);
			// validate after a POST
		//	if(SmartyValidate::is_valid($_POST))
		//	{	// no errors, done with SmartyValidate
		//		SmartyValidate::disconnect();

				if( isset($_POST['submit']) )
				{	$info = array(
						"user"	=> $default['username'],
						"expires" => strftime($format, locker_expires()),
						"phone"	=> $_POST['locker_current_phone'],
						"combo"	=> $_POST['locker_combo']
						);
					if( $message = $this->db->signup($_POST['locker_id'], $info) )
					{	$this->view->message = $message;
						$_POST['expires'] = $info['expires'];
					}
				}
			//}

			$this->view->assign($_POST);
		//}
	}
}

