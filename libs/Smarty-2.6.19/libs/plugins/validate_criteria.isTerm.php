<?php

 /**
 * test if a value is a valid term number
 *
 * @param string $value the value being tested
 * @param array params validate parameter values
 * @param array formvars form var values
 */

function smarty_validate_criteria_isTerm($value, &$params, &$formvars)
{
    if(strlen($value) == 0)
        return false;

	return ereg('^[0-9]{3}[1|5|9]$', $value);
}

?>
