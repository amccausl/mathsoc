<div id='main-header'><h1></h1></div>
<div class="section">
  <h3>Missing Hours</h3>
  <p>The following users have signed up for a position that requires an office hour, but don't have one yet.</p>
  <ul>
{foreach from=$missing item=person}
    <li>{$person.name} - {$person.positions}</li>
{/foreach}
  </ul>
  <h3>Emails</h3>
<pre>
{foreach from=$missing item=person}"{$person.name}" &lt;{$person.email}&gt;,
{/foreach}
</pre>
</div>
