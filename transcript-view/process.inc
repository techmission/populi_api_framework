<?php

/* Processes the transcript data for display - 
   calculating SAP, etc. */
function process_transcript_data($transcript_data) {

  /* Step 1: Finish filling in the data on courses. */
  $transcript_data = populate_course_data($transcript_data);

  /* Step 2. Do the running calculations */
  $transcript_data = calculate_totals($transcript_data);
  
  /* Step 3. Make the SAP determinations */
  $transcript_data = calculate_sap($transcript_data);
  
  return $transcript_data;
}

function populate_course_data($transcript_data) {
  // subtract out the PLA credits from the transfer credits for calculation
  $pla_credits = 0;
  if(count($transcript_data['transcripts'] > 0)) {
    foreach($transcript_data['transcripts'] as $transcript) {
      if($transcript['name'] == 'Prior Learning Assessment') {
        $pla_credits = $transcript['transfer_credit'];
      }
    }
  }
  if($pla_credits > 0) {
   $transcript_data['overall']['transfer_credit_for_gpa'] = $transcript_data['overall']['transfer_credit'] - $pla_credits;
  }
  else {
    $transcript_data['overall']['transfer_credit_for_gpa'] = $transcript_data['overall']['transfer_credit'];
  }

  // list the grades where no credits are earned
  $grades_zero_credits = array('F', 'WF', 'I');
  
  // Iterate over the transcript's terms and courses to populate data needed for display
  // and for the later calculation steps
  foreach($transcript_data['terms'] as $term_name => $term) {
    foreach($term['courses'] as $course_name => $course) {
      // set to not applicable for courses in progress or withdrawn
      if(empty($course['grade']) || $course['grade'] == 'W') {
        if(empty($course['grade'])) {
          $transcript_data['terms'][$term_name]['courses'][$course_name]['grade'] = 'IP'; // in progress
        }
        // Set all to n/a
        $transcript_data['terms'][$term_name]['courses'][$course_name]['course_gpa'] = 'n/a';
        $transcript_data['terms'][$term_name]['courses'][$course_name]['credits'] = 'n/a';
        $transcript_data['terms'][$term_name]['courses'][$course_name]['credits_earned'] = 'n/a';
        $transcript_data['terms'][$term_name]['courses'][$course_name]['quality_points'] = 'n/a';
        // Skip the rest of the logic for this course
        continue;
      }
      // correct the number of credits attempted, such as for Capstone
      // note that credits override data is inaccurate
      $credits_attempted = get_course_credits($course['name']);
      $transcript_data['terms'][$term_name]['courses'][$course_name]['credits'] = $credits_attempted;
      // Reset credits earned 
      $credits_earned = 0;
      // Reset quality points
      $quality_points = 0;
      $course_gpa = $transcript_data['terms'][$term_name]['courses'][$course_name]['course_gpa'];
      if(in_array($course['grade'], $grades_zero_credits)) {
        $credits_earned = 0;
        $transcript_data['terms'][$term_name]['courses'][$course_name]['credits_earned'] = $credits_earned;
      }
      else {
       $credits_earned = $credits_attempted; 
       $transcript_data['terms'][$term_name]['courses'][$course_name]['credits_earned'] = $credits_earned;
      }
      $quality_points = $credits_earned * $course_gpa; 
      $transcript_data['terms'][$term_name]['courses'][$course_name]['quality_points'] = $quality_points;  
    }
  }
  return $transcript_data;
 }
 
function calculate_totals($transcript_data) {
  // Set up transfer credit & GPA
  if(empty($transcript_data['overall']['transfer_credit'])) {
    $transfer_credit = 0;
  }
  else {
    $transfer_credit = $transcript_data['overall']['transfer_credit_for_gpa'];
  }
  if(empty($transcript_data['overall']['transfer_gpa'])) {
    $transfer_gpa = 0;
  }
  else {
    $transfer_gpa = $transcript_data['overall']['transfer_gpa'];
  }
  
  // Initialize variables
  $cvu_credits_attempted = 0;
  $cvu_credits_earned = 0;
  $cvu_quality_points = 0;      // Quality points used in GPA calculation
  $total_credits_attempted = 0;
  $total_credits_earned = 0;
  $cvu_gpa = 0;
  $total_gpa = 0;
  $courses_excluded = array();  // Courses that were excluded from the GPA
  $credits_excluded = 0;
  
  // Iterate through the terms to get the data up to that point,
  // and progressively set the overall data
  foreach($transcript_data['terms'] as $term_name => $term) {
    // Reset the GPAs to be calculated each time
    // The credits earned accumulate
    $cvu_gpa = 0;
    $total_gpa = 0;
    // Iterate through the courses, to sum up for that term
    foreach($term['courses'] as $course_name => $course) {
      // TODO: determine how to handle P's (only for core courses that are transfers)
      // probably just skip those, like W's & IP's
      if($course['grade'] != 'W' && $course['grade'] != 'IP') {
        // TODO: determine whether I need to handle credits override
        $cvu_credits_attempted = $cvu_credits_attempted + $course['credits'];
        // remove the earlier attempt from the credits attempted if this course had been excluded earlier
        if((in_array($course['name'], array_keys($courses_excluded))) && $course['course_gpa'] > 0) {
          $credits_excluded = $credits_excluded + $courses_excluded[$course['name']]['credits'];
          $transcript_data['terms'][$term_name]['courses'][$course_name]['is_repeat'] = TRUE;
          unset($courses_excluded[$course['name']]);
        }
        else {
          $transcript_data['terms'][$term_name]['courses'][$course_name]['is_repeat'] = FALSE;
        }
        $cvu_credits_earned = $cvu_credits_earned + $course['credits_earned'];
        $cvu_quality_points = $cvu_quality_points + $course['quality_points'];
        // If this is an excluded course, add it to the excluded courses array
        if($course['exclude'] == TRUE) {
          $courses_excluded[$course['name']] = array('name' => $course['name'], 'credits' => $course['credits']);
        }
      }
      else {
        continue;
      }
    }  
    // The total number of credits attempted is the City Vision credits attempted plus the transfer credit
    // Note that we don't have a record in here of courses they attempted but which we didn't accepted for transfer credit
    $total_credits_attempted = $cvu_credits_attempted + $transfer_credit;
    // See comment above
    $total_credits_earned = $cvu_credits_earned + $transfer_credit;   
      
    $cvu_gpa = $cvu_quality_points / ($cvu_credits_attempted - $credits_excluded);
    //$debug[] = (array('term_name' => $term_name, 'cvu_quality_points' => $cvu_quality_points, 'credits_excluded' => $credits_excluded)); 

    // Calculate total GPA with transfer credit if there is any
    if($transfer_credit > 0) {
      // This formula is documented on http://www.cityvision.edu/city-vision-gpa-calculation-procedure
      // Also in the spreadsheet from Spring 1
      $total_gpa = (($cvu_gpa * ($cvu_credits_attempted - $credits_excluded)) + ($transfer_credit * $transfer_gpa)) / ($total_credits_attempted - $credits_excluded);
    }
    // Otherwise, total GPA is same as City Vision GPA
    else {
      $total_gpa = $cvu_gpa;
    }
      
    // Save the data into the transcript_data array for the term
    $transcript_data['terms'][$term_name]['cvu_credit_attempted'] = $cvu_credits_attempted;
    $transcript_data['terms'][$term_name]['cvu_credit_earned'] = $cvu_credits_earned;
    $transcript_data['terms'][$term_name]['total_credit_attempted'] = $total_credits_attempted;
    $transcript_data['terms'][$term_name]['total_credit_earned'] = $total_credits_earned;
    $transcript_data['terms'][$term_name]['cvu_gpa'] = $cvu_gpa;
    $transcript_data['terms'][$term_name]['total_gpa'] = $total_gpa;
    
    // Save the data into the transcript_data array for overall
    $transcript_data['overall']['cvu_credit_attempted'] = $cvu_credits_attempted;
    $transcript_data['overall']['cvu_credit_earned'] = $cvu_credits_earned;
    $transcript_data['overall']['total_credit_attempted'] = $total_credits_attempted;
    $transcript_data['overall']['total_credit_earned'] = $total_credits_earned;
    $transcript_data['overall']['cvu_gpa'] = $cvu_gpa;
    $transcript_data['overall']['total_gpa'] = $total_gpa;
  }
  //log_to_screen($debug);
  return $transcript_data;
}
 
function calculate_sap($transcript_data) {
   // empty SAP status
   $sap_status = '';
   // At the beginning, everyone is making SAP
   $prev_sap_status = STATUS_MAKING_SAP;
   // At first, no one has completed terms
   $num_completed_terms = 0;
   foreach($transcript_data['terms'] as $term_name => $term) {
    // Check to see if this is a completed term (based on course completions)
    // If so, increment the counter on completed terms
    if(is_term_completed($term)) {
      $num_completed_terms++;
    }
     // determine the current SAP status by looking at terms to date
    $sap_status = get_sap_status_by_history($prev_sap_status, $term['total_gpa'], $term['total_credit_earned'], $term['total_credit_attempted'], $num_completed_terms);
     // set the SAP status for this term
    $transcript_data['terms'][$term_name]['sap_status'] = $sap_status;
    $transcript_data['overall']['sap_status'] = $sap_status;
    // Set the previous sap status to the one that was just determined,
    // so that the next one can be calculated
    $prev_sap_status = $sap_status;
  }
  log_to_screen($transcript_data);
  return $transcript_data;
}
 
 /* Determines what the SAP should be based on previous SAP */
 function get_sap_status_by_history($prev_sap_status, $gpa, $credits_earned, $credits_attempted, $num_completed_terms) { 
   $current_sap_status = '';
   
   $making_sap = is_making_sap($gpa, $credits_earned, $credits_attempted, $num_completed_terms);
   
   // FIXME: would need to find some way to account for SAP appeals, but they are not recorded in the DB
   // Until then, will have to manually correct for anyone who was granted an appeal

   // TODO: add code to handle so that it doesn't change the SAP status for
    // a term that is still in progress
   if($prev_sap_status == STATUS_MAKING_SAP && $making_sap == TRUE) {
     $current_sap_status = STATUS_MAKING_SAP;
   }
   else if($prev_sap_status == STATUS_MAKING_SAP && $making_sap == FALSE) {
     $current_sap_status = STATUS_WARNING;
   }
   else if($prev_sap_status == STATUS_WARNING && $making_sap == TRUE) {
     $current_sap_status = STATUS_MAKING_SAP;
   }  
   else if($prev_sap_status == STATUS_WARNING && $making_sap == FALSE) {
     $current_sap_status = STATUS_BLOCKED_PENDING_APPEAL;
   }
   else if($prev_sap_status == STATUS_BLOCKED_PENDING_APPEAL && $making_sap == TRUE) {
     $current_sap_status = STATUS_MAKING_SAP;
   }
   else if($prev_sap_status == STATUS_BLOCKED_PENDING_APPEAL && $making_sap == FALSE) {
     $current_sap_status = STATUS_BLOCKED;
   }
   else if($prev_sap_status == STATUS_BLOCKED && $making_sap == TRUE) {
     $current_sap_status = STATUS_MAKING_SAP;
   }
   else if($prev_sap_status == STATUS_BLOCKED && $making_sap == FALSE) {
     $current_sap_status = STATUS_BLOCKED;
   }
   
   return $current_sap_status;
 }
   
 /* Determines whether the student is making SAP by the first 2 criteria */
 function is_making_sap($gpa, $credits_earned, $credits_attempted, $num_completed_terms) {
   // TODO: add logic to handle 150% time (rarely is that invoked, so probably not crucial)
   $is_making_sap = FALSE;
   
   /* Step 1: GPA metric */
   $is_making_sap_gpa = get_sap_gpa($gpa, $credits_attempted, $num_completed_terms);
   
   /* Step 2: Pace of completion metric */
   $is_making_sap_pace = get_sap_pace($credits_earned, $credits_attempted);
   
   // return TRUE if matching on both criteria
   if($is_making_sap_gpa && $is_making_sap_pace) {
     $is_making_sap = TRUE;
   }
   else {
     $is_making_sap = FALSE;
   }
   return $is_making_sap;
 }
 
 /* GPA metric */
 function get_sap_gpa($gpa, $credits_attempted, $num_completed_terms) {
   // don't count repeated courses in credits attempted
   // use total GPA
   
   $is_making_sap_gpa = FALSE;
   
   // counting as less than 2 years any students that have completed 10 or less terms,
   // even if those are non-consecutive
   if($num_completed_terms <= 10) {
     if($credits_attempted <= 30 && $gpa >= 1.5) {
       $is_making_sap_gpa = TRUE;
     }
     else if($credits_attempted >= 31 && $credits_attempted <= 59 && $gpa >= 1.7) {
       $is_making_sap_gpa = TRUE;
     }
     else if($credits_attempted >= 60 && $credits_attempted <= 89 && $gpa >= 1.85) {
       $is_making_sap_gpa = TRUE;
     }
     else if($credits_attempted >= 90 && $gpa >= 2) {
       $is_making_sap_gpa = TRUE;
     }
   }
   else {
     if($gpa >= 2) {
       $is_making_sap_gpa = TRUE;
     }
   }
   
   return $is_making_sap_gpa;
 }
 
/* Pace of completion metric */
function get_sap_pace($credits_earned, $credits_attempted) {
  $is_making_sap_pace = FALSE;
   
  // calculate pace (as a percentage)
  $pace = $credits_earned / $credits_attempted * 100;
   
  if($credits_attempted <= 30 && $pace >= 50) {
    $is_making_sap_pace = TRUE;
  }
  else if($credits_attempted >= 31 && $credits_attempted <= 59 && $pace >= 60) {
    $is_making_sap_pace = TRUE;
  }
  // Note that the website says 66 2/3rds
  else if($credits_attempted >= 60 && $pace >= 66.67) {
    $is_making_sap_pace = TRUE;
  }
     
  return $is_making_sap_pace;
}

/* Check if term is completed by checking for course completions */
function is_term_completed($term) {
  $is_completed_term = FALSE;
  $course_completions = 0;
  foreach($term['courses'] as $course_name => $course) {
    if($course['credits_earned'] > 0) {
      $course_completions++;
    }
  }
  if($course_completions > 0) {
    $is_completed_term = TRUE;
  }
  return $is_completed_term;
}

function get_course_credits($course_name) {
  $course_credits = CREDITS_DEFAULT;
  // Override for Capstone Project Design and Capstone
  // TODO: don't use "magic numbers"
  if(strpos('Capstone Project Design', $course_name) !== FALSE) {
    $course_credits = 1;
  }
  // TODO: handle the 8-credit Capstone Project (somehow?)
  else if(strpos('Capstone Project', $course_name) !== FALSE) {
    $course_credits = 5;
  }
  return $course_credits;
}
