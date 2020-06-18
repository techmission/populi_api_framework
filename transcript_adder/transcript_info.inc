<?php

/* Program ids */
function getProgramId($name) {
  $id = NULL;
  if($name == "Undergraduate") {
    $id = 25207;
  }
  else if($name == "Graduate") {
    $id = 25213;
  }
  return $id;
}

/* Course groups */
function getCourseGroup($name) {
  $course_groups = array(
    'Electives' => 99661,
    'General Education Electives (includes all general education courses)' => 100381,
    'General Education: Broad Knowledge and Intellectual Skills' => 130051,
    'General Education: Christian Life, Thought and Social Responsibility' => 130057, 
    'General Education: Communications' => 99667,
    'Humanities/Fine Arts' => 99649,
    'Math/Natural Science' => 99655
  );
  if(array_key_exists($name, $course_groups)) {
    return $course_groups[$name];
  }
  else {
    return NULL;
  }
}


/* Grade options */
function getGradeOption($label) {
  $grade_options = array(
    'A' => 97,
    'A-' => 91,
    'B+' => 88,
    'B' => 85,
    'B-' => 82,
    'C+' => 78,
    'C' => 75,
    'C-' => 72,
    'D+' => 68,
    'D' => 65,
    'D-' => 62,
    'F' => 0,
    'P' => 80,
    'Withdrawn' => 'W',
    'Incomplete' => 'I',
    'WP' => 'CUSTOM_STUDENT_STATUS_2785',
    'WF' => 'CUSTOM_STUDENT_STATUS_2791',
    'W (unofficial)' => 'CUSTOM_STUDENT_STATUS_3049',
    'NS' => 'CUSTOM_STUDENT_STATUS_3139'
  );
  if(array_key_exists($label, $grade_options)) {
    return $grade_options[$label];
  }
  else {
    return NULL;
  }
}

/* look up a single person by first and last name */
function getPersonId($firstname, $lastname) {
    // 1) look up student id by first & last name
    // use method call to look up students by first name, lastname & get ids
    $fullname = $firstname . ' ' . $lastname;
    $students = $populi->getPossibleDuplicatePeopleByName($firstname, $lastname);
    if(is_array($students) && count($students) > 0) {
      $student_id = $students[0]['id'];
      script_log(print_r($students[0], TRUE), LEVEL_DEBUG);
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
    return $students[0]['id'];
}
