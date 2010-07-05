<div id='main-header'><h1></h1></div>
<div class="section">
  <p>Locker Sign up page, please click on the locker you want.  Lockers in red are already taken.</p>
  <map name="damap">
{foreach from=$lockers key=locker item=coords}
    <area shape='rect' href='{$baseUrl}/Lockers/Signup?locker_id={$locker}' coords='{$coords[0]},{$coords[1]},{$coords[2]},{$coords[3]}'/>
{/foreach}
  </map>
  <img usemap='#damap' src='{$smarty.server.REQUEST_URI}&format=.png' border=0 />
  <br/>
  <a href='{$baseUrl}/Lockers/signup'>Return</a>
</div>
