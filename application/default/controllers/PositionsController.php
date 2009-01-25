<?php

require_once 'MathSocAction.inc';

require_once 'userDB.inc';

class PositionsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new UserDB();
	}

	/** /positions/ - Display current positions and who holds them
	 *
	 */
	public function indexAction()
	{	// Lookup current positions from database
		$positions = $this->db->getPositionsByCategory();

		// Add positions to the view
		$this->view->assign($positions);
	}

	/** /positions/details/:name - display detailed information for a given position
	 */
	public function detailsAction()
	{	$position = $this->db->getPosition( $this->_getParam('position') );
		$this->view->position = $position;
	}

	public function directorshipsAction()
	{	$positions = $this->db->getAvailablePositions();
		$directorships = $positions['DIR'];
		$this->view->directorships = $directorships;
	}

	/** /positions/apply
	 * Allow users to apply for a position
	 */
	public function applyAction()
	{
		if( !isset( $_POST['term'] ) )
		{	$terms = array();
			$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
			$seasons = array( "W", "S", "F" );
			for( $i = 0; $i < 3; $i++ )
			{	$terms[$term] = $seasons[floor($term%10/4)] . " " . (floor($term/10) + 1900);
				if( $term % 10 == 9 )
					$term += 2;
				else
					$term += 4;
			}
			$this->view->term_options = $terms;
		}elseif( !isset( $_POST['position'] ) )
		{	// Present available positions
			$positions = $this->db->getAvailablePositions( $_POST['term'] );
			$positions = $positions['DIR'];
			$this->view->positions =  $positions;

			$options = array();
			foreach( $positions as $position )
			{	$options[$position['alias']] = $position['name'];
			}
			$this->view->positions_options = $options;
		}elseif( !isset( $_POST['submit'] ) )
		{	// Add fields for the position from the database
			$questions = $this->db->getPositionQuestions( $this->_getParam( 'position' ) );
			$this->view->questions = $questions;
		}else
		{	// Process the form
			$application = array(
							'term' => $_POST['term'],
							'unit' => 'mathsoc',
							'position' => $_POST['position'],
							'user' => Zend_Auth::getInstance()->getIdentity());

			// Grab the answers to the custom questions
			$questions = array();
			foreach( $_POST as $key => $value )
			{	if( substr( $key, 0, 2 ) == 'q_' )
					$questions[ substr( $key, 2 ) ] = $value;
			}

			$application['questions'] = serialize( $questions );

			$this->db->addApplication( $application );
		}
	}
}

