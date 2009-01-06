<?php
/**
 * Smarty plugin
 * @package Smarty
 * @subpackage plugins
 */


/**
 * Smarty {term} function plugin
 *
 * Type:     function<br>
 * Name:     implode<br>
 * Purpose:  glue an array together as a string, with spupplied string glue, and assign it to the template<br>
 * @link http://smarty.php.net/manual/en/language.function.implode.php {implode}
 *       (Smarty online manual)
 * @author Will Mason <will at dontblinkdesign dot com>
 * @param array
 * @param Smarty
 */
function smarty_function_term($params, &$smarty)
{
    if( isset($params['id']) )
	{	$term = $params['id'];
    }else
	{	$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
	}
	
	if( $params['display'] == "long" )
	{	$seasons = array( "Winter", "Spring", "Fall" );
    }elseif( $params['display'] == "id" )
	{	return $term;
	}else
	{	$seasons = array( "W", "S", "F" );
	}
	$term = $seasons[floor($term%10/4)] . " " . (floor($term/10) + 1900);
	
	return $term;
}

/* vim: set expandtab: */

