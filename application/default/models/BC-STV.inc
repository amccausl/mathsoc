<?php

/***** BC-STV.php -- Version 1.0 *****
 * Writen by Alex McCausland
 * 
 * This file is meant to tally given votes from an online election using
 * the BC-STV voting system.  More information about this system can be
 * found at www.citizensassembly.bc.ca.  You can use this by passing an
 * array of votes to BC_STV along with the number of positions to run the
 * election for.
 *
 * ChangeLog:
 *  2007-01-30 - 1.0
 *    Initial Release
 *
 * Future Improvements:
 *    -	array_fill_keys should be included in the standard php soon.  This
 *	method won't be necessary when that happens. (It looks like this happened)
 *    - add a graphical tracing feature with GD (just for swag factor)
 */

// Fill an arrays keys with the values passed
//function array_fill_keys(array $keys, $value)
//{	return array_combine($keys, array_fill(0, count($keys), $value));
//}

// Remove the $remove array from the $vote
function array_diff_recursive( &$vote, $key, $remove )
{	$vote = array_diff( $vote, $remove );
}

// Helper function to be called by array_walk
function BC_STV_reweight( &$vote, $key, $transfer=1 )
{
	if( !isset( $vote['transfer'] ) )
	{	$vote['transfer'] = $transfer;
		
	}else
	{	// Set the new transfer value of this vote
		$vote['transfer'] *= $transfer;
	}

	if( count( $vote ) == 0 )
		$vote['transfer'] = 0;
}

// Helper function to be called by array_reduce
function BC_STV_tally( $sum, $vote )
{	$sum += $vote['transfer'];
	return $sum;
}

/** BC_STV - This tallys the votes with the BC-STV method
 *
 * @param   votes	An associated array containing the ranking of the candidates on several different votes
 * @param   position_count  The number of positions being contested between the candidates
 * @param   validate	Set this to true to trace the code
 * @return		An array with the number of votes cast and the winner(s) of the election
 */
function BC_STV( $votes, $position_count = 1, $validate=false )
{
	// Element 0 in the array is the CROs vote
	$CRO_ballot = array_shift( $votes );

	// The CROs ballot must contain a ranking of all the candidates,
	// so the candidates list can be retrieved from that.
	$candidates = $CRO_ballot;

	// Declare the transfer values for all the votes
	array_walk( $votes, 'BC_STV_reweight' );

	// Initialize the outcome of the election
	// The first element of $outcome is the number of votes cast
	$outcome = array( count( $votes ) );

	// Calculate the number of votes needed for election
	$votes_needed = (int)( count( $votes ) / ( $position_count + 1 ) ) + 1;

	// Initialize tally with candidates
	$tally = array_fill_keys( $candidates, array() );

	if( $validate )
	{	print( "# of votes: {$outcome[0]}\n" );
		print( "# of positions: {$position_count}\n" );
		print( "votes needed: {$votes_needed}\n" );
	}

	// Count the top ranking on all the votes cast and add them to their candidates vote piles
	foreach( $votes as $vote )
	{	if( count( $vote ) > 1 )
		{	//$vote = array_values( $vote );
			//$vote['transfer'] = array_pop( $vote );

			if( in_array( $vote[0], $candidates ) && isset( $tally[$vote[0]] ) )
				array_push( $tally[$vote[0]], $vote );
		}
	}

	// Run loop while there are positions to be filled
	while( count( $outcome ) <= $position_count )
	{
		// If there are enough positions for candidates, they can be acclaimed
		if( count( $candidates ) == 0 )
		{	return $outcome;
		}elseif( count( $candidates ) <= $position_count - count( $outcome ) + 1 )
		{	return array_merge( $outcome, $candidates );
		}

		if( $validate )
		{	print( "The current tally is:\n" );
			print_r( $tally );
		}

		$winners = array();
		$standing = array();
		$leftovers = array();
		$minValue = $votes_needed;

		if( $validate )
		{	print( "candidates:\n" );
			print_r( $candidates );
		}

		foreach( $candidates as $candidate )
		{	
			// Add up the votes for each candidate
			if( empty( $tally[$candidate] ) )
				$standing[$candidate] = 0;
			else
				$standing[$candidate] = array_reduce( $tally[$candidate], 'BC_STV_tally' );
			

			// elect candidates if they have enough votes
			if( $standing[$candidate] >= $votes_needed )
			{	array_push( $winners, $candidate );

				$surplus = $standing[$candidate] - $votes_needed;
				$transfer_rate = $surplus / $standing[$candidate];
				array_walk( $tally[$candidate], 'BC_STV_reweight', $transfer_rate );
				$leftovers = array_merge( $leftovers, $tally[$candidate] );
				$candidates = array_diff( $candidates, array($candidate) );
				$candidates = array_values( $candidates );

				if( $validate )
				{	print "$candidate has {$standing[$candidate]} >= {$votes_needed} votes\n";
					print "*** $candidate has been elected\n";
				}

			}else
			{	if( $standing[$candidate] < $minValue )
				{	$minValue = $standing[$candidate];
				}
				if( $validate )
				{	print "$candidate has {$standing[$candidate]} < {$votes_needed} votes\n";
				}
			}
		}

		// If no one has been elected this round, we must eliminate a candidate.
		if( empty( $winners ) )
		{
			// We must find the candidate with the fewest number of votes who is lowest on the CRO's ballot
			$i = count( $candidates ) - 1;
			while( $standing[$candidates[$i]] != $minValue )
			{	$i--;
				if( $i < 0 )
					print "HUGE PROBLEM!!!!  I'm insane!\n";
			}
			// $candidates[i] is the candidate with the fewest 1st ranking
			//votes that has the lowest ranking in the CROs ballot (tie breaking method)
			$dropped = $candidates[$i];
			// Remove the dropped candidate from the list of candidates
			$candidates = array_diff( $candidates, array($dropped) );
			// Renumber the candidates
			$candidates = array_values( $candidates );
			// Collect the dropped persons votes into the leftovers that will be reassigned to other candidates
			$leftovers = $tally[$dropped];
			// drop the tally for this dropped candidate
			unset( $tally[$dropped] );

			if( $validate )
			{	print "The standings are [$i dropped]:\n";
				print_r( $standing );
				print "the CRO ballot is:\n";
				print_r( $CRO_ballot );
				print "minValue = {$minValue}\n*** {$dropped} has been dropped from the running\n";
			}
		}else
		{
			// Remove everyone who already has a seat from the leftover votes
			array_walk( $leftovers, 'array_diff_recursive', $winners );
		}

		// For each of the leftover votes, reassign to other candidates
		foreach( $leftovers as $vote )
		{	// Renumber the elements of the array, make sure dropped candidates don't screw up ordering
			$vote = array_values( $vote );
			$vote['transfer'] = array_pop( $vote );

			// while the first ranking isn't a candidate anymore, keep removing them
			while( !in_array( $vote[0], $candidates ) && !empty( $vote ) )
				array_shift( $vote );
				
			// double check sanity and add users vote to their new preferences tally
			if( in_array( $vote[0], $candidates ) && isset( $tally[$vote[0]] ) )
				array_push( $tally[$vote[0]], $vote );
		}

		// add the winners to the outcome of the election
		$outcome = array_merge( $outcome, $winners );

		if( $validate )
		{	print( "Outcome:\n" );
			print_r( $outcome );
		}
	}

	return $outcome;
}

/* TESTING SUITE FOR VOTE TALLYING
 * Executed if run from the terminal.
 */

if( eregi( basename(__FILE__), $_SERVER['PHP_SELF'] ) )
{
	$votes = array();
	$candidates = array('alex', 'natalie', 'andrew', 'G', 'derek');
	shuffle( $candidates );
	array_push($votes, $candidates);		// Add the CRO's ballot
	$position_count = rand( 1, 6 );

	// Generate random votes
	$count = rand(30, 150);
	while( --$count > 0 )
	{	shuffle( $candidates );
		$vote = $candidates;
		$ranking = rand( 1, count($candidates) );
		while( --$ranking > 0 )
		{	array_shift( $vote );
		}
		array_push( $votes, $vote );
	}

	// Output votes
	//print_r( $votes );

	// Count votes
	$outcome = BC_STV( $votes, $position_count );

	// Output results
	print( "# positions: $position_count\n" );
	print_r( $outcome );
}

?>
