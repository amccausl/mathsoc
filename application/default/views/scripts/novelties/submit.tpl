<div id="main-header"><h1></h1></div>
<div class="section">
  <p>Welcome to MathSoc's t-shirt design contest.  By submitting a design to our contest, you are transferring ownership to the Math Society.</p>
  <div class="form-container">
    <form action="{$smarty.server.REQUEST_URI}" enctype="multipart/form-data" method="post">
    <fieldset>
      <legend>Novelty Design Submission</legend>
      <label for="email">Email:</label>
        <input name="email" type="text" length="30" disabled="true" value="{$email}" /><br/>
      <label for="name">Design Name:</label>
        <input name="name" type="text" length="31" value="{$name}" /><br/>
      <label for="description">Description for your submission:</label>
        <textarea name="description" cols="40" rows="5">{$description}</textarea>
      <label for="tshirt_front">T-Shirt Image (Front)</label>
        <input type="file" name="tshirt_front" size="20" value="{$tshirt_front}" />
      <label for="tshirt_back">T-Shirt Image (Back)</label>
        <input type="file" name="tshirt_back" size="20" value="{$tshirt_back}" />
      <input type="hidden" name="notes" value="MathSoc T-Shirt Design Contest (Winter 2009)"/>
    </fieldset>

    <div class="buttonrow">
      <input name="submit_tshirt" type="submit" value="Submit Design" class="button"/> 
    </div>
    </form>
  </div>
</div>
