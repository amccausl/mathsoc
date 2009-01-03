<div id='main-header'><h1></h1></div>
<div class="section">
  <h2>{$user.name}</h2>
{$user.profile|indent:4}

{foreach from=$user.current item=position}
  Display current position information and emails
  ie {$position.name}
{/foreach}

<!-- TODO: add office schedule image if user has an hour -->

  <h3>Position History</h3>
  <ul>
{foreach from=$user.terms key=term item=positions}
	{implode subject=positions glue=', ' assign=positions}
    <li>{$term} - {$positions}</li>
{foreachelse}
    <li>User has held no positions</li>
{/foreach}
  </ul>
</div>
