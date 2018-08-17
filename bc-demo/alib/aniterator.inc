<?php
  /**
   * iterate over an array.
   * When you reach one end, you loop back to the other.
   * @package aLib
   * @subpackage Utility
   */
class anIterator extends anObject {
  protected $_data = array();
  protected $pointer;
  protected $size;
  private $evercalled;
  public $noLoop = FALSE;
    
  function __construct($rawData = ""){
        
    if($rawData != ""){
      $this->init($rawData);
    }
    parent::__construct();
  }
    
  /**
   * initialize the object with a fresh array of data
   * @param array $rawData the array of data
   */
  public function init($rawData){
    $this->pointer = 0;
    $this->evercalled = 0;
    $this->_data = $rawData;
    $this->size = sizeof($rawData) - 1;
  }
    
  /**
   * Return the next element in the array, or the first if at the end of the array.
   */
  public function next(){
    $current = $this->_data[$this->pointer];
    $this->pointer++;
    if($this->pointer > $this->size){
      if ($this->noLoop) {
	$this->_data[] = FALSE; // This doesn't affect things if we later decide to loop since it's beyond the scope of this->size
      } else {
      $this->reset();
      }
    }
    if ($this->evercalled == 0) {
      $this->evercalled = 1;
    }
    return $current;
  }

  /* Prev is what makes this confusing. So here's an example.
   data is [a,b,c,d,e]
   size is 4 (5-1)
   pointer is 0;
  
   $this->next returns "a" and sets the pointer at 1 (b);
   $this->next again returns "b" and sets the pointer at 2 (c);
   $this->prev then subtracts the pointer twice, which is 0, so it returns "a", then sets the pointer to 1. "a" *is* prev to "b".
   $this->next then returns "b" and sets the pointer to 2 (c);
  
  
  */
  
  /**
   * Return the prev element of the array, or the last element if
   * currently looking at the first.
   */

  function prev(){
    if ($this->evercalled == 0) {
      $this->evercalled = 1;
      $current = $this->_data[$this->size];
      return $current;
    } else {
      $this->pointer--;
      if($this->pointer < 0) {
	if ($this->noLoop) {
	  return FALSE;
	}
	$this->pointer = $this->size;
      }
      $this->pointer--; #Yes, twice.
      if($this->pointer < 0) {
	if ($this->noLoop) {
	  return FALSE;
	}
	$this->pointer = $this->size;
      }
      $current = $this->_data[$this->pointer];
      $this->pointer++;
      return $current;
    }
  }   
    
  /**
   * Reset the iterator to the first element of the array.
   */
  function reset(){
    $this->pointer = 0;
  }


  /**
   *
   */
  function dump () {
    return $this->_data;

  }

}

?>
