<?php 

/* This is a system to be coupled with a uwdir authentication scheme

*/

class NoveltiesDB
{
	protected $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	function getNovelties()
	{	$query = "SELECT * FROM novelties";
		$novelties = $this->_db->fetchAll( $query );
		return $novelties;
	}

	function getNovelty($id)
	{	$query = "SELECT *
				  FROM novelties
				  WHERE id = ?";
		$novelty = $this->_db->fetchRow( $query, $id );

		$query = "SELECT name, image, type
				  FROM novelties_images
				  WHERE id = ?";
		$images = $this->_db->fetchAll( $query, $id );

		$novelty['images'] = $images;

		return $novelty;
	}

	function submitNovelty( $novelty )
	{
		// Extract the images from the novelty item
		$images = $novelty['images'];
		unset( $novelty['images'] );

		// Insert the novelty to the database
		$this->_db->insert('novelties', $novelty);

		$id = $this->_db->lastInsertId();

		// Insert the images that associate with the novelty
		foreach( $images as $image )
		{	$image['id'] = $id;
			$this->_db->insert('novelties_images', $image);
		}
	}
}
