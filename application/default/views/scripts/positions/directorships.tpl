<div id='main-header'><h1></h1></div>
<div class="section">
  <h3>Directorships</h3>
  <p>Most of MathSoc's directorships are a term committement and are appointed by and report to the VPAS.  The Internal and External Directors are appointed by and report to the President.  The Resources director is appointed by and reports to the VPA.  To apply for a directorship, you can drop by the MathSoc office (MC 3038), or email {mailto address="vpas@mathsoc.uwaterloo.ca"} for more information.</p>
  <ul>
{foreach from=$directorships item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> :
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{foreachelse}
    <li>All directorship positions are filled this term.</li>
{/foreach}
  </ul>

</div>
