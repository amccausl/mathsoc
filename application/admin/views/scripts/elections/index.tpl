<div class="section">
{foreach from=$elections key=id item=info name=loop}
{if $smarty.now > info.polls_close}
{* If the poll is closed, display vote tally and information *}
{else}
{/if}
{/foreach}
<!-- TODO: add link to declare election -->
</div>
