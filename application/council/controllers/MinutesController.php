<?php

require_once 'MathSocAction.inc';
require_once 'minutesDB.inc';

/**
 */

class Council_MinutesController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();
		$this->initView();

		$this->db = new MinutesDB();
	}

	public function indexAction()
	{	$minutes = $this->db->getMinutes();
		$this->view->minutes = $minutes;
	}

	public function displayAction()
	{	if( $this->_getParam("id")
			&& $minute = $this->db->getMinutes($this->_getParam("id")) )
		{	Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);
			
			if( $minute['format'] == 'text' )
			{	header( "Content-type: text/plain" );
			}elseif( $minute['format'] == 'html' )
			{	header( "Content-type: text/html" );
			}
			print( $minute['minutes_text'] );
		}
	}
}
