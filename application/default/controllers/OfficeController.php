<?php

require_once 'MathSocAction.inc';
require_once 'officeDB.inc';

class OfficeController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new OfficeDB();
	}

	/** Present information about the services offered in the office
	 *
	 */
	public function indexAction()
	{
	}

	/** Present a clickable image of the office worker schedule and form for user information if necessary
	 *
	 */
	public function signupAction()
	{	// Authenticate the user
		$auth = Zend_Auth::getInstance();
		if( !$auth->hasIdentity() )
		{	$this->_redirect('auth/login');
		}

		// TODO: if the user hasn't volunteered before, present form

		// TODO: Present imagemap

		// TODO: Process post, if user information set, insert/update user

		// Sign the user up for the hour
		// TODO: add result to view
		if( $this->db->signup( $auth->getIdentity(), $this->_getParam('hourid') ) )
		{
		}else
		{
		}
	}

	/** Display an image of the office hour schedule
	 *
	 */
	public function hoursAction()
	{
		// Set information for the schedule image
		$height = 350;
		$width = 562;
		//$height = .7*800;     //For printing
		//$width = .7*1035;
		$x['start'] = 40;
		$y['start'] = 20;
		$x['delta'] = 145 - $x['start'];
		$y['delta'] = 37;
		$x['entries'] = 5;
		$y['entries'] = 9;

		$map = array();

		// Create image map for the image
		$key = 0;

		for( $u = 0; $u < $x['entries']; $u++ )
		{
			for( $v = 0; $v < $y['entries']; $v++ )
			{	$key++;
				$x_1 = $x['start'] + ( $x['delta'] * $u );
				$x_2 = $x['start'] + ( $x['delta'] * ($u+1) );
				$y_1 = $y['start'] + ( $y['delta'] * $v );
				$y_2 = $y['start'] + ( $y['delta'] * ($v+1) );
				$map[$key] = array( $x_1, $y_1, $x_2, $y_2 );
			}
		}

		$this->view->map = $map;

		if( $this->_getParam('format') )
		{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);
			require_once( 'schedule.inc' );

			$days = $this->db->getSchedule();

			$xml = 
"<?xml version='1.0' encoding='ISO-8859-1' ?>
<schedule blocks='8:25,9:25,10:25,11:25,12:25,13:25,14:25,15:25,16:25' days='Monday,Tuesday,Wednesday,Thursday,Friday'>
  <settings>
    <legend font='2' leftpadding='40' toppadding='20' rightpadding='0' bottompadding='0'/>
    <block bgcolor='#efefef' bordercolor='#0099dd' leftpadding='5' toppadding='3' alpha='40%'>
      <title align='center' color='#0' font='3'/>
      <description align='center' color='#0' font='2'/>
    </block>
  </settings>
";

			if( isset( $_GET['names'] ) )
			{
				foreach( $days as $day => $hours )
				{
					$xml .= "  <day number='{$day}'>\n";
					foreach($hours as $hour => $users)
					{
						$start = $hour - 7;
						$xml .= "    <block at='{$hour}' blockoffset='-{$span}' blockspan='0' bgcolor='#FFC0FF' alpha='30%' name='";
						$names = array();
						foreach($users as $people)
						{	$name = split( " ", $people['name'] );
							array_push($names, $name[0]);
						}
						$xml .= implode( ", ", $names);
						$xml .= "'/>\n";
					}
					$xml .= "  </day>\n";
				}
			}else
			{
				foreach( $days as $day => $hours )
				{
					$xml .= "  <day number='{$day}'>\n";

					$start = -1;
					$span = 1;

					foreach($hours as $hour => $users)
					{
						$hour = $hour - 7;
						if( $start + $span == $hour )
						{	$span++;
						}else
						{	$hour++;
							$xml .= "    <block at='{$hour}' blockoffset='-{$span}' blockspan='0' bgcolor='#FFC0FF' alpha='30%' name=''/>\n";
							$span = 1;
						}
					}
					$xml .= "  </day>\n";
				}
			}

			$xml .= "</schedule>";

			$sch = new Schedule($xml, false);
			// change size of image
			$sch->width = isset($_GET['width']) ? $_GET['width'] : $width;
			$sch->height = isset($_GET['height']) ? $_GET['height'] : $height;
			// now draw ;)
			$sch->Draw();
		}else
		{
		}
	}
}
