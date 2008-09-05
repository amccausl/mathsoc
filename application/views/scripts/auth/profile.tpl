<div id="main">
  <!-- <div id='main-header'><h1></h1></div> -->
  <div class="section">
    <h2>{$user.name}</h2>
{$user.profile|indent:4}

{foreach from=$user.current item=position}
    Display current position information and emails
    ie {$position.name}
{/foreach}

{foreach from=$user.past item=position}
    Display previous positions and terms
    {$position.name}
{/foreach}
  </div>
</div>
