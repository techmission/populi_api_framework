<?php

/* Main business logic function: queries Salesforce and populates the transcript data array */
function get_transcript_data($student_id) {
  // Create empty shell of array
  // NOTE: Would be better as class but would take longer to code
  $transcript_data = build_transcript_data_array();

  // 1. Query to get student's basic information
  $transcript_data['studentinfo'] = get_student_info($student_id);

  // 2a. Query to get student's transfer credit & transfer credit GPA, if any
  $transfer_credit_data = get_transfer_credit_data($student_id);
  if(!empty($transfer_credit_data['transfer_credit']) && !empty($transfer_credit_data['transfer_gpa'])) {
    $transcript_data['overall']['transfer_credit'] = $transfer_credit_data['transfer_credit'];
    $transcript_data['overall']['transfer_gpa'] = $transfer_credit_data['transfer_gpa'];
  }

  // 2b. Query to get the related transcript objects, if any
  $transcript_data['transcripts'] = get_transcripts($student_id);

  // 3. Query to get the course registrations
  $transcript_data['terms'] = get_course_registrations_by_term($student_id);
  
  return $transcript_data;
}

/* Step 0: Looks up a Populi student record based on either email 
   or first name + last name */
function get_student_id($query_type, $fields) {
  // Use our existing Populi connection
  global $POPULI;
  $student_id = '';

  if($query_type == QUERY_STUDENTNAME) {
    // wraps the Populi method that returns Id's by student name
    $students = $POPULI->getPossibleDuplicatePeopleByName($fields['FirstName'], $fields['LastName']);
    // always return the first one (for now)
    if(is_array($students) && count($students >= 1) {
      $student_id = $students[0]['Id'];
    }
  }
  else if($query_type == QUERY_EMAIL) {
    // TODO: wrap a method that returns Id's by email
    // need to write that in framework
  }
  return $student_id;
}

/* Step 1: Get the basic information on the student */
function get_student_info($student_id) {
  // Build the "shell"
  $student_info = array(
    'firstname' => '',
    'lastname' => '',
    'major' => '',
    'program' => ''
  );
  // TODO: determine what methods are needed to populate these fields
  // TODO: change the results criteria
  if(isset($results[0]->Id) && !empty($results[0]->Id)) {
   $student_info['firstname'] = $results[0]->FirstName;
   $student_info['lastname'] = $results[0]->LastName;
   $student_info['major'] = $results[0]->City_Vision_Program_Track__c;
   $student_info['program'] = $results[0]->City_Vision_Program_Interest__c;
  }
  return $student_info;
}

/* Step 2a: Get the student's overall transfer credit data */
function get_transfer_credit_data($student_id) {
  // Build the "shell"
  $transfer_credit_data = array(
    'transfer_credit' => 0,
    'transfer_gpa' => NULL,
  );
  // TODO: determine how to populate these fields
  if(isset($results[0]->Id) && !empty($results[0]->Id)) {
   $transfer_credit_data['transfer_credit'] = $results[0]->CVC_TC_Total_Transfer_Credits__c;
   $transfer_credit_data['transfer_gpa'] = $results[0]->CVC_TC_Transfer_Credit_GPA__c;
  }
  return $transfer_credit_data;
}

/* Step 2b: Get the student's transcripts from other schools */
function get_transcripts($student_id) {
  // Build the "shell"
  $transcripts = array();
  // TODO: determine how to get the data on each
  if(isset($results[0]->Id) && !empty($results[0]->Id)) {
    foreach($results as $result) {
      $transcripts[] = array(
        'name' => $result->Name,
        'transfer_credit' => $result->Credits__c,
        'transfer_gpa' => $result->GPA_on_Accepted_Credits__c
      );
    }
  }
  return $transcripts;
}

/* Step 3: Get the student's course registrations by term */
function get_course_registrations_by_term($sfid) {
  // Build the "shell"
  $terms = array();
  // TODO: replace this with the logic to get the actual info
  if(isset($results[0]->Id) && !empty($results[0]->Id)) {
    foreach($results as $result) {
      $term_name = $result->Year__c . " (" . $result->Term_Sorted__c . ")";
      // Add the human-readable term name
      if(!isset($terms[$term_name])) {
        // trim as per https://stackoverflow.com/questions/4915753
        $term_trimmed = substr($result->Term__c, 0, -5);
        $term_plus_year = $term_trimmed . " " . $result->Year__c;
        $terms[$term_name]['name'] = $term_plus_year;
        // look up the term dates based on the term name
        $terms[$term_name]['term_dates'] = get_term_date($term_plus_year);
      }
      // Add the actual course registration data
      $course_name = $result->Name;
      $terms[$term_name]['courses'][$course_name] = array(
        'name' => $course_name,
        'grade' => $result->Grade__c,
        'course_gpa' => $result->Course_GPA__c,
        'credits' => $result->Number_of_Credits__c,                   // handle in business logic if needs an override    
        'credits_override' => $result->Number_of_Credits_Override__c, // handle in business logic: maybe ignore? (bad data)
        'masters' => $result->Is_Masters_Course__c,                   // handle in business logic: probably ignore
        'exclude' => $result->Exclude_from_GPA__c,                    // handle in business logic
        'last_wk_completed' => $result->Last_Week_Completed__c,       // handle in business logic
      );
    }
  }
  return $terms;
}


/* Utility functions */

/* Validates whether data is in the correct format to render */
function is_transcript_valid($transcript_data) {
  if(is_array($transcript_data) && count($transcript_data) > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
  // TODO: add more validation in here, such as checking for non-zero values
  // for overall data
}

/* Creates shell of transcript data array */
function build_transcript_data_array() {
  $transcript_data = array();
  $transcript_data['studentinfo'] = array();
  $transcript_data['overall'] = array(
    'transfer_credit' => 0,
    'transfer_gpa' => NULL,
    'city_vision_credit' => 0,
    'city_vision_gpa' => NULL,
    'total_credit' => 0,
    'total_gpa' => NULL
  );
  $transcript_data['transcripts'] = array(); 
  $transcript_data['terms'] = array();
  return $transcript_data;
}

