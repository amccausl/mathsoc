<div class="section">
  <script language='javascript'>

  function setDefault(form)
  {ldelim}
    switch( form.position.value )
    {ldelim}
{foreach from=$positions item=p name=loop}
      case '{$p.alias}':
        form.p_name.value = "{$p.name}";
        form.p_desc.value = "{$p.description}";
        break;
{/foreach}
    {rdelim}

    switch( form.position.value )
    {ldelim}
      case 'new':
        form.p_name.style.visibility='visible';
        break;
      default:
        form.p_name.style.visibility='hidden';
    {rdelim}
  {rdelim}

  </script>
  <p>This page allows you to create a new election. You need to know when
    you would like nominations to open and close, when you would like
    voting to start and close, and you must have a file that contains all
    the uwuserids of the people you would like to be able to vote for as a
    CSV (or comma separated values).</p>
  <div class='form-container'>
    <form action='{$smarty.server.REQUEST_URI}' method='post'>
    <fieldset>
      <legend>Position Information</legend>
      <label for='position'>Position</label>
        {html_options name=position options=$position_options selected=$position onchange='setDefault(this.form)'}
      <input id='p_name' name='p_name' type='text' size='30'/><br/>
      <label for='p_desc'>Description</label>
      <textarea id='p_desc' name='p_desc' rows='5' cols='40'></textarea><br/>
	</fieldset>
    <fieldset>
      <legend>Nomination Information</legend>
      <label for='n_required'>Nominations Required</label>
      <input id='n_required' name='n_required' type='text' value='10'/><span class='hint'>(-1 disables online nominations)</span><br/>
      <label for='n_open'>Open Nominations</label>
        {html_select_date prefix='nominations_open_' display_years=false time=$nominations_open}
        {html_select_time prefix='nominations_open_' use_24_hours=true display_seconds=false time=$nominations_open}<br/>
      <label for='n_close'>Close Nominations</label>
        {html_select_date prefix='nominations_close_' display_years=false time=$nominations_close}
        {html_select_time prefix='nominations_close_' use_24_hours=true display_seconds=false time=$nominations_close}<br/>
    </fieldset>
    <fieldset>
      <legend>Voting Information</legend>
      <label for='voters' class='required'>Voter List</label>
      <input id='voters' name='voters' type='file'/><br/>
      <label for='v_open' class='required'>Open Polls</label>
        {html_select_date prefix='polls_open_' display_years=false time=$polls_open}
        {html_select_time prefix='polls_open_' use_24_hours=true display_seconds=false time=$polls_open}<br/>
      <label for='v_close' class='required'>Close Polls</label>
        {html_select_date prefix='polls_close_' display_years=false time=$polls_close}
        {html_select_time prefix='polls_close_' use_24_hours=true display_seconds=false time=$polls_close}<br/>
    </fieldset>
    <input id='submit' name='submit' type='submit' value='Declare Election'/>
  </div>
</div>
