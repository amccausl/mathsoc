<div id='main-header'><h1></h1></div>
<div class="section">
<h2>Course Evaluations</h2>
<p>Every term, the Faculty of Mathematics solicits student feedback of its courses aind instructors.  They have made these evaluations available to you.</p>

<table border="1" cellpadding="0" cellspacing="0" width="100%">
<thead><tr>
  <th width="33%">Term</th>
  <th width="33%">Instructors</th>
  <th width="33%">TAs</th>
</tr></thead>
<tbody>
{foreach from=$evals item=eval}
<tr>
  <td align="center">
	{if $eval.term % 10 == 1}Winter {/if}
	{if $eval.term % 10 == 5}Spring {/if}
	{if $eval.term % 10 == 9}Fall {/if}
  	{math equation="floor(x/10)+1900" x=$eval.term}
  </td>
  <td align="center">{if $eval.prof_file}<a href='{$baseUrl}/courseevals/profs/{$eval.term}'>Download</a>{/if}</td>
  <td align="center">{if $eval.ta_file}<a href='{$baseUrl}/courseevals/tas/{$eval.term}'>Download</a>{/if}</td>
</tr>
{/foreach}
</tbody>
</table>
</div>
