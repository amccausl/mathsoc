<div class="section">
  Locker Sign up page, please click on the locker you want.<br/>
  <map name="damap">
{foreach from=$lockers key=locker item=coords}
    <area shape='rect' href='{$baseUrl}/Lockers/Signup?locker={$locker}' coords='{$coords[0]},{$coords[1]},{$coords[2]},{$coords[3]}'/>
{/foreach}
  </map>
  <img usemap='#damap' src='{$smarty.server.REQUEST_URI}&format=.png' border=0 />
  <br/>
  <a href='{$baseUrl}/Lockers'>Return</a>
</div>
