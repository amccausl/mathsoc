<div id='main-header'><h1></h1></div>
<div class="section">

  <h3>Current Hours</h3>
  <form action='{$smarty.server.REQUEST_URI}' method='post'>
  <ul>
{foreach from=$hours_held key=day item=hours}
    <li>{$day} -
	  <ul>
	{foreach from=$hours key=key item=hour}
		<li>{$hour} - <input type="submit" name="drop{$key}" value="Drop Hour"/></li>
	{/foreach}
	  </ul>
{foreachelse}
  <li>You do not currently have any office hours.</li>
{/foreach}
  </ul>
  </form>
  
  <p>If you would like to sign up for an hour, you can click on the hour slot of the schedule below.  If you haven't been office trained, you should email {mailto address=office@mathsoc.uwaterloo.ca} and they will be happy to get you set up.</p>
  <map name='office'>
{foreach from=$map key=hour item=coords}
    <area shape='rect' href='{$baseUrl}/Office/signup?hour={$hour}' coords='{$coords[0]},{$coords[1]},{$coords[2]},{$coords[3]}'/>
{/foreach}
  </map>
  <img usemap='#office' src='{$baseUrl}/Office/hours?names&format=.png'/>
</div>
