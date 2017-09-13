<?php

/* Extension class with wrapper methods for Populi functions. */
class Populi extends PopuliCore {
  /* wrapper around the getPossibleDuplicatePeople call to get an
     array of students, with their key information, but matching on the
     firstname and lastname */
  public function getPossibleDuplicatePeopleByName($first_name, $last_name, $gender = NULL, $birth_date = NULL, $email_address = NULL, $phone_number = NULL) {
    // Pass the parameters as array
    // https://stackoverflow.com/questions/1419467
    $params = get_defined_vars();  
    // Request the result. Not raw, since needed as SimpleXML.
    try {	  
      $result = $populi->doTask('getPossibleDuplicatePeople', $params, FALSE);
    }
    catch(PopuliException $ex) {
      script_log($ex->getMessage() . ' : ' . $ex->getPopuliCode, LEVEL_ERROR);
      // Return NULL if error on Populi side so calling code knows there was an API error, instead of no results.
      return NULL;
    }
    // Define the array that will be returned if there are any matching students.
    // The number of array members is the number of matches on first and last name.
    $people = array();
    /* Walk the SimpleXML and populate the array,
       only adding where people's names match the passed in values. */
    foreach($possible_duplicate as $person) {
      // Include all variations of capitalization - cf. https://stackoverflow.com/questions/5473542
      if((strcasecmp((string) $person->first_name, $first_name) == 0) &&
        (strcasecmp((string) $person->last_name, $last_name) == 0)) {
	// Convert the address to array for ease of handling.
	$address = (array) $person->primary_address;
        $people[] = array(
          'id' => (int) $person->id,
	  'first_name' => (string) $person->first_name,
	  'last_name' => (string) $person->last_name,
	  'preferred_name' => (string) $person->preferred_name,
	  'birthdate' => (string) $person->birthdate,
	  'primary_email' => (string) $person->primary_email,
	  'primary_phone' => (string) $person->primary_phone,
	  'primary_address' => $address
	);
      }
    }
    return $people;
  }

  /* Wrapper around the addTag API call */
  public function addTag($person_id, $tag_id = NULL, $tag = NULL) {
    $params = get_defined_vars();  
    // Return error if no tag_id or tag passed
    if($tag_id == NULL && $tag == NULL) {
       script_log('No tag_id or tag passed to add_tag', LEVEL_ERROR);
    }
    try {
      $response = $populi->doTask('addTag', $params, FALSE);
    }
    catch(PopuliException $ex) {
      script_log($ex->getMessage() . ' : ' . $ex->getPopuliCode, LEVEL_ERROR);
    }
    // Return TRUE or FALSE based on success
    if((string) $response == 'SUCCESS') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
	    
  /* Wrapper around the removeTag API call */
  public function removeTag($person_id, $tag_id = NULL, $tag = NULL) {
    $params = get_defined_vars();  
    // Return error if no tag_id or tag passed
    if($tag_id == NULL && $tag == NULL) {
       script_log('No tag_id or tag passed to remove_tag', LEVEL_ERROR);
    }
    try {
      $response = $populi->doTask('removeTag', $params, FALSE);
    }
    catch(PopuliException $ex) {
      script_log($ex->getMessage() . ' : ' . $ex->getPopuliCode, LEVEL_ERROR);
    }
    // Return TRUE or FALSE based on success
    if((string) $response == 'SUCCESS') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
 }
