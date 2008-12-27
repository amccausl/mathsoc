<!--<div id='main-header'><h1></h1></div>-->
<div class="section">
  <p>If you wish to sign up for a MathSoc office hour, you can do it by clicking on the hour you wish to sign up for below.  If you are having problems or wish to drop an hour, send me an email at website@mathsoc.uwaterloo.ca</p>
  <map name='office'>
{foreach from=$map key=hour item=coords}
    <area shape='rect' href='{$baseUrl}/Office/signup?hour={$hour}' coords='{$coords[0]},{$coords[1]},{$coords[2]},{$coords[3]}'/>
{/foreach}
  </map>
  <img usemap='#office' src='{$smarty.server.REQUEST_URI}?format=.png'/>
</div>
