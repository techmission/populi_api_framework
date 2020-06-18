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
    'A' => 97.5,
    'A-' => 91.5,
    'B+' => 88.5,
    'B' => 85.5,
    'B-' => 82,
    'C+' => 78.5,
    'C' => 75.5,
    'C-' => 72,
    'D+' => 68.5,
    'D' => 65.5,
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
