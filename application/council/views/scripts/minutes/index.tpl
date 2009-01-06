<div id='main-header'><h1></h1></div>
<div class="section">
  <h2>MathSoc Council Minutes</h2>
{foreach from=$minutes key=term item=meetings}
  <h3>{term id=$term display="long"}</h3>
  <ul>
  {foreach from=$meetings key=id item=meeting}
    <li><a href="{$baseUrl}/council/minutes/{$id}">Meeting {$meeting.meeting_number} ({$meeting.meeting_date})</a></li>
  {/foreach}
  </ul>
{/foreach}
</div>
