<?php 

// This snippet is from http://www.hawkee.com/snippet/2238/

// This line must be used to load the Text/Diff.php PEAR module
// This can be downloaded from http://pear.php.net/package/Text_Diff
require_once("Text/Diff.php");

// Object created like this:
// $diff = new diff($from_text,$to_text);
// $from_text is the original text
// $to_text is the modified text
class diff extends Text_MappedDiff {

  public function diff($from_text,$to_text) {
  #public function __construct($from_text,$to_text) {
      
    // Split the text into words
    $from_words = preg_split('/\b/',$from_text);
    $to_words = preg_split('/\b/',$to_text);
    
    // These alternate split statements will split the text into individual characters
    //$from_words = str_split($from_text,1);
    //$to_words = str_split($to_text,1);
    
    // These split statements will split the text into lines
    //$from_words = explode("\n",$from_text);
    //$to_words = explode("\n",$to_text);
    
    // Text_MappedDiff requires copies of these variables
    $mapped_from_words = $from_words;
    $mapped_to_words = $to_words;
    
    // Call the construct method of the parent class
    parent::Text_MappedDiff($from_words,$to_words,$mapped_from_words,$mapped_to_words);
  }
  
  // Renders the difference
  public function render() {
    $difference = $this->getDiff(); // Calls the getDiff() method, part of Text_MappedDiff
    $render = ''; // This will be the rendered difference.
    foreach ($difference as $op) {
      // $op is the operation getDiff() has calculated.
      // You can tell which operation (add, delete or change) $op is by checking which class the object is
      if ($op instanceof Text_Diff_Op_copy) { // This uses the instanceof operator which is only in PHP5+, in PHP4 you can use is_a()
        // Text was copied (not changed), add it to the rendered difference unmodified
        $render .= implode('',$op->final);
      } elseif ($op instanceof Text_Diff_Op_delete) {
        // Text was deleted, add it to the rendered difference in red
        $render .= '<span style="background-color: #FF0000;">' . implode('',$op->orig) . '</span>';
      } elseif ($op instanceof Text_Diff_Op_add) {
        // Text was added, add it to the rendered difference in green
        $render .= '<span style="background-color: #00FF00;">' . implode('',$op->final) . '</span>';
      } elseif ($op instanceof Text_Diff_Op_change) {
        // Text was modified, add it to the rendered difference as if the original text was deleted and the new text was added
        $render .= '<span style="background-color: #FF0000;">' . implode('',$op->orig) . '</span>' . 
          '<span style="background-color: #00FF00;">' . implode('',$op->final) . '</span>';
      }
    }
    return $render;
  }
}

?>
