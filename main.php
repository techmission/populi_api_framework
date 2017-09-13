<?php

/* Main script that does the tagging */

/* Require includes */
$base_path = dirname(__FILE__);

// database connection
require_once $base_path . 'connection.inc';
// Populi API settings
require_once $base_path . 'api_settings.inc';
// other constants (if any)
require_once $base_path . 'constants.inc';

/* Settings that can change */

// Tag names
require_once $base_path . 'tags.inc';
// Other settings (such as debug mode)
require_once $base_path . 'settings.inc';

/* Require libraries */

// PDO functions
require_once $base_path . 'pdo.inc';
// Populi functions
require_once $base_path . 'populi.inc';
// Logging functions
require_once $base_path . 'logging.inc';

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
    /*
    // 1) look up student id by first & last name
    // use method call to look up students by first name, lastname & get ids
    $fullname = $result['firstname'] . ' ' . $$result['lastname'];
    $students = $populi->getPossibleDuplicatePeopleByName($first_name, $last_name]);
    // For now, skip duplicates.
    if(is_array($students) && count($students)> 1) {
      script_log($fullname . ' had ' . $count . ' duplicates.', LEVEL_ERROR);
      continue;
    }
    // If there are no matches found, then log and continue.
    if($students == FALSE) {
      script_log($fullname . ' had no matches found.', LEVEL_ERROR);
    }

    // 2) update tags
    foreach($tags as $tag_name => $tag) {
      // 2a) remove all tags of the requisite ids
       (since there’s no other way to keep in sync)
      removeTag (whatever that is)

      //  2b) add the correct tags for the student
     addTag(studentId, tagId) ?
    }
    */
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
