<?php

/* Main script that does the tagging */

/* Require includes */
$base_path = dirname(__FILE__);

require_once $base_path . 'includes.inc';

// Run the tagging
tag_students();

function tag_students() {
  // Establish database connection
  $db = db_connect();

  // Establish Populi API connection
  $populi = new Populi();
  // Use default login credentials
  $populi->login();

  // Use PDO db query
  try {
    $stmt = $db->query('SELECT * FROM report_for_tagging');
    $results = $stmt->fetchAll(PDO::FETCH_ASSOC);
  }
  catch(PDOException $ex) {
    script_log($ex->getMessage(), LEVEL_ERROR);
  }
  
  // Get the names of the tags
  $tags = get_tags();

  // Iterate over results to apply tags
  $count = 0;
  foreach($results as $result) {
    // 1) look up student id by first & last name
    // use method call to look up students by first name, lastname & get ids
    $fullname = $result['firstname'] . ' ' . $$result['lastname'];
    // TODO: determine if capitalization will be an issue
    $students = $populi->getPossibleDuplicatePeopleByName($first_name, $last_name]);
    if(is_array($students) && count($students) == 1)) {
      $student_id = $students[0]['id'];
    }
    // For now, skip duplicates and log.
    else if(is_array($students) && count($students) > 1) {
      script_log($fullname . ' had ' . $count . ' duplicates.', LEVEL_ERROR);
      continue;
    }
    // If there are no matches found, then log and continue.
    else if(is_array($students) && count($students) = 0) {
      script_log($fullname . ' had no matches found.', LEVEL_ERROR);
    }
    // Method returns NULL if Populi has an error
    else if($students == NULL) {
      script_log('Populi error in getPossibleDuplicatePeople call', LEVEL_ERROR);
    }
    // This should never happen.
    else {
      script_log('unknown error in getPossibleDuplicatePeopleByName', LEVEL_ERROR);
    }

    // 2) update tags
    foreach($tags as $tag_name => $tag) {
      // 2a) remove all tags of the requisite ids
       (since there’s no other way to keep in sync)
      removeTag (whatever that is)

      //  2b) add the correct tags for the student
     addTag(studentId, tagId) ?
    }
    // exit after 1 student processed
    if(SCRIPT_MODE == MODE_DEBUG && $count > 0) {
      break;
    }
    $count++;
  }
  $tagging_result = $count . " students tagged.";
  echo $tagging_result;
  script_log($tagging_result, LEVEL_DEBUG);
  return $count;
}  
