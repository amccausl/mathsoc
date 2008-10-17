<?php

 /**
 * test if a value is a valid course code
 *
 * @param string $value the value being tested
 * @param array params validate parameter values
 * @param array formvars form var values
 */

function smarty_validate_criteria_isCourseCode($value, &$params, &$formvars)
{
    if(strlen($value) == 0)
        return false;

	return ereg('^[0-9]{3}[A-Z|a-z]*$', $value);
}
/*
if( !smarty_validate_criteria_isCourseCode("241", $num = 4, $value = 3) )
	print( "failed1\n" );

if( !smarty_validate_criteria_isCourseCode("241A", $num = 4, $value = 3) )
	print( "failed2\n" );
*/
?>
