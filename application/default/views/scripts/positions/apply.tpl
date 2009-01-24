<div id="main-header"><h1></h1></div>
<div class="section">
  <h2>Apply for a Position</h2>
  <p>This is where people can apply for available positions.  <a href="{$baseUrl}/positions/nominations">View elected positions here.</a></p>
  <!-- display form for user to apply, store username, alias in drop down, link to elections -->
  <form action='{$smarty.server.REQUEST_URI}' method='post'>
  <fieldset>
    <legend>Application Information</legend>
    <label for="position">Position</label>
      {html_options name=position options=$positions_options selected=$position}<br/>
    <label for="term">Term</label>
      {html_options name=term options=$term_options selected=$term}<br/>
{foreach from=$questions item=question}
    <label for="q_{$question.key}">{$question.text}</label>
{if $question.type == 'text'}
      <textarea name="q_{$question.key}">{$question.default}</textarea>
{elseif $question.type == 'radio'}
{/if}
{/foreach}
    <input type="submit" name="submit" value="Submit" /></td>
  </fieldset>
  </form>
</div>
