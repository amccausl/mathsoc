<div id='main-header'><h1></h1></div>
<div class="section">
  <h2>{$user.name}</h2>
{$user.profile|indent:4}

{implode subject=$user.current glue=", " assign=positions}
  <h3>Current Positions</h3>
  <p>{if $positions}{$positions}{else}This user doesn't currently have any positions.{/if}</p>

  <h3>Office Hours</h3>
  <img src='{$baseUrl}/office/hours/?width=450&height=260&username={$user.userId}&format=.png'/>

  <h3>Position History</h3>
  <ul>
{foreach from=$user.terms key=term item=positions}
	{implode subject=$positions glue=', ' assign=positions}
    <li>{term id=$term display="long"} - {$positions}</li>
{foreachelse}
    <li>User has held no positions</li>
{/foreach}
  </ul>
</div>
