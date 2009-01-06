<?php

require_once 'MathSocAction.inc';

class WebsiteController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = false)
	{	parent::init($secure);
	}

	public function indexAction()
	{	require_once( "form.inc" );
		if( isset($_POST['submit']) )
		{	$this->view->message = "Thank you for your feedback";

			$_POST['date'] = date("m/d H:i:s");
			unset( $_POST['submit'] );
			
			$fh = fopen( "../data/comments", "a" );
			fputs( $fh, serialize( $_POST ) );
			fclose( $fh );
		}
	}
}

