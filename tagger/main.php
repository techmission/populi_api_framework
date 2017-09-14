<?php

/* Main script that does the tagging */

/* Require includes */
$base_path = dirname(__FILE__);

// Core includes
require_once $base_path . '/../includes.inc';

// Settings specific to this script
require_once $base_path . '/tagger/settings.inc';

// Functions specific to tagging
require_once $base_path . '/tagger/tags.inc';

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
  $count = 0;  // Number of students processed
  $tags_added = 0; // Number of tags added
  $tags_removed = 0; // Number of tags removed
  foreach($results as $result) {
    // 1) look up student id by first & last name
    // use method call to look up students by first name, lastname & get ids
    $fullname = $result['firstname'] . ' ' . $result['lastname'];
    $students = $populi->getPossibleDuplicatePeopleByName($result['firstname'], $result['lastname']);
    if(is_array($students) && count($students) == 1) {
      $student_id = $students[0]['id'];
      script_log(print_r($students[0], TRUE), LEVEL_DEBUG);
    }
    // For now, skip duplicates and log.
    else if(is_array($students) && count($students) > 1) {
      script_log($fullname . ' had ' . $count . ' duplicates.', LEVEL_ERROR);
      continue;
    }
    // If there are no matches found, then log and continue.
    else if(is_array($students) && count($students) == 0) {
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
    /* 2) update tags for the student */
    // 2a) remove all tags of the requisite ids
    // (since there's no other way to keep them in sync)
    foreach($tags as $tag_name => $tag_id) {
      /* 2a) remove all tags of the requisite ids
       (since there’s no other way to keep in sync) */
      $response = $populi->removeTag($student_id, $tag_id);
      // Debugging
      if($response == TRUE) {
       script_log('removed tag ' . $tag_name . ' from student ' . $fullname, LEVEL_DEBUG);
       $tags_removed++;
      }
      else {
        script_log('failed to remove ' . $tag_name . ' from student ' . $fullname, LEVEL_ERROR);
      } 
    }

    // call function that determines which tags to add based on result
    $tags_to_add = get_tags_to_add($result);

    // 2b) add the correct tags for the student
    if(is_array($tags_to_add) && count($tags_to_add) > 0) {
      foreach($tags_to_add as $tag_id) {
        $response = $populi->addTag($student_id, $tag_id);
        if($response == TRUE) {
         script_log('added tag ' . $tag_id . ' to student ' . $fullname, LEVEL_DEBUG);
        }
        else {
          script_log('failed to add ' . $tag_id . ' to student ' . $fullname, LEVEL_ERROR);
        }
        $tags_added++;
      }
    }
    // exit after 3 students processed
    if(SCRIPT_MODE == MODE_DEBUG && $count > 3) {
      break;
    }
    $count++;
  }
  $students_processed = $count . " students processed." . PHP_EOL;
  $tags_removed_result = $tags_removed . " tags removed." . PHP_EOL;
  $tags_added_result = $tags_added . " tags added." . PHP_EOL;
  $tagging_result = $students_processed . $tags_added_result . $tags_removed_result;
  echo $tagging_result;
  script_log($tagging_result, LEVEL_DEBUG);
  return $count;
} 

function get_tags_to_add($student) {
  // If bad data is passed, then skip.
  if(!is_array($student) || (!isset($student['firstname']) || !isset($student['lastname'])) || (empty($student['firstname']) || empty($student['lastname']))) {
    script_log('bad data call to get_tags_to_add' . PHP_EOL . print_r($student, TRUE), LEVEL_ERROR);
    return array();
  }
  $tags = array();
  if($student['completed_vfao'] == 1) {
    $tag_id = get_tag_id_by_name('Completed VFAO Interview');
    if(is_numeric($tag_id)) {
      $tags[] = $tag_id;
    }
  }
  if($student['pell_1617'] == 1) {
    $tag_id = get_tag_id_by_name('Pell 16-17');
    if(is_numeric($tag_id)) {
      $tags[] = $tag_id;
    }
  }
  if($student['loans_1617'] == 1) {
    $tag_id = get_tag_id_by_name('Loan 16-17');
    if(is_numeric($tag_id)) {
      $tags[] = $tag_id;
    }
  }
  if($student['has_isir'] == 1) {
    $tag_id = get_tag_id_by_name('17-18 ISIR');
    if(is_numeric($tag_id)) {
      $tags[] = $tag_id;
    }
  }
  return $tags;
}
