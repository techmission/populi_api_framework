<?php


// TODO: see if this is still useful in this context
// was originally created to flatten SF objects
function flatten_object($object) {
  // Start with an empty object.
  $flat_obj = new stdClass();
  //print_r($object);
  if(is_object($object)) {
    // Cast to array for ease of iteration.
    $array = (array) $object;
	foreach($array as $idx => $sub_array) {
	  if(!is_array($sub_array) && !is_object($sub_array)) {
	    continue;
	  }
	  else {
	    if(is_object($sub_array)) {
	      $sub_array = (array) $sub_array;
		}
	    foreach($sub_array as $fieldname => $field) {
		  // If a regular field, flatten it up a level.
		  if(!is_array($field)) {
		    // TODO: see if this naming convention should be changed
		    $array[$idx . '__' . $fieldname] = $field;
		  }
		  // Don't operate down to the next level of depth.
		  else {
		    continue;
		  }
		}
	  }
	}
	// Cast back to object at end.
	$flat_obj = (object) $array;
  }
  return $flat_obj;
}

// TODO: see if still needed when I wrote a Populi logging framework
function log_to_screen($var) {
  global $VARS;
  $VARS .= print_r($var, TRUE) . '<br/><br/>';
  return;
}
