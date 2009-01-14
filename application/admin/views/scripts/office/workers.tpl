<div id='main-header'><h1></h1></div>
<div class="section">
  <h3>Current Office Workers</h3>
  <p>This is the page to get information about current office workers.</p>
  <img src='{$baseUrl}/office/hours?names&width=460&format=.png'/>
  
  <h3>Emails</h3>
<pre>
{foreach from=$workers item=person}"{$person.name}" &lt;{$person.email}&gt;,
{/foreach}
</pre>
</div>
