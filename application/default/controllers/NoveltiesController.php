<?php

require_once 'MathSocAction.inc';

class NoveltiesController extends MathSoc_Controller_Action
{
	public function indexAction()
	{
	}

	public function contestAction()
	{
	}

	public function submitAction()
	{	require_once( "../application/default/views/helpers/form.inc" );
		$this->secure();

		$novelty = array(
				'submitter'	=> Zend_Auth::getInstance()->getIdentity(),
				'name'		=> $_POST['name'],
				'description' => $_POST['description'],
				'notes'		=> $_POST['notes'],
				'images'	=> array(),
			);

		if( isset( $_POST['submit_tshirt'] ) )
		{	$novelty['style'] = 'T-Shirt';
			
			if( $_FILES['tshirt_front']['error'] == UPLOAD_ERR_OK )
			{	$front = array(
					'name'	=> 'Front',
					'type'	=> $_FILES['tshirt_front']['type'],
					'image'	=> file_get_contents( $_FILES['tshirt_front']['tmp_name'] )
				);
				array_push( $novelty['images'], $front );
			}

			if( $_FILES['tshirt_back']['error'] == UPLOAD_ERR_OK )
			{	$back = array(
					'name'	=> 'Back',
					'type'	=> $_FILES['tshirt_back']['type'],
					'image'	=> file_get_contents( $_FILES['tshirt_back']['tmp_name'] )
				);
				array_push( $novelty['images'], $back );
			}

			$this->db->submitNovelty( $novelty );
		}
	}
}

