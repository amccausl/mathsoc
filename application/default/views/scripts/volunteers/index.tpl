<div id='main-header'><h1></h1></div>
<div class="section">
  <p>All of MathSoc's services are provided by volunteers.  Without their help, the society wouldn't be able to offer any of our wonderful services to the students of the math faculty.</p>

  <p>This terms volunteers are included below in alphabetical order with the positions they are helping us with.  If you see one about, stop them to thank them for their time.<p>

  <ul>
{foreach from=$volunteers key=userId item=user}
{implode subject=$user.positions glue=", " assign=$positions}
    <li><a href="{$baseUrl}/user/profile/?username={$userId}">{$user.name}</a> - {$position}</li>
{/foreach}
  </ul>
</div>
