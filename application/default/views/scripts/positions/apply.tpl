<div id="main-header"><h1></h1></div>
<div class="section">
  <h2>Apply for a Position</h2>
  <p>This is where people can apply for available selected positions.  If you would like to apply for one of the elected positions, you can <a href="{$baseUrl}/positions/nominations">view elected positions here.</a></p>
  <form action='{$smarty.server.REQUEST_URI}' method='post'>
  <fieldset>
{if $term_options}
    <legend>Term Select</legend>
    <label for="term">Term</label>
      {html_options name=term options=$term_options selected=$term}<br/>
    <input type="submit" name="submit_term" value="Get Available Positions" />
{elseif $smarty.post.submit_term}
<script language="javascript">
function setDescription(form)
{ldelim}
	switch( form.position.value )
	{ldelim}
{foreach from=$positions item=position}
		case '{$position.alias}':
			document.getElementById('p_description').innerHTML = '{$position.description|escape:'quotes'|regex_replace:"/[\r\t\n]/":" "}';
			break;
{/foreach}
	{rdelim}
{rdelim}
</script>
    <legend>Position Select</legend>
    <label for="position">Position</label>
      {html_options id=position name=position options=$positions_options selected=$position onchange='setDescription(this.form)'}<br/>
	<label for="p_description">Description:</label><br/>
	  <span id="p_description" name="p_description"></span><br/>
	<input type="hidden" name="term" value="{$smarty.post.term}"/>
    <input type="submit" name="submit_position" value="Apply For Position" />
{elseif $smarty.post.submit_position}
    <legend>Application Information</legend>
  {foreach from=$questions item=question}
    <label for="q_{$question.key}">{$question.text}</label>
    {if $question.type == 'text'}
      <textarea name="q_{$question.key}">{$question.default}</textarea>
    {elseif $question.type == 'radio'}
    {/if}
  {/foreach}
	<input type="hidden" name="term" value="{$smarty.post.term}"/>
	<input type="hidden" name="position" value="{$smarty.post.position}"/>
    <input type="submit" name="submit" value="Submit Application" />
{/if}
  </fieldset>
  </form>
</div>
