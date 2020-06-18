<?php
// probably make this a shebang /usr/bin/env php file

/* Main script that does the tagging */

/* Require includes */
$base_path = dirname(__FILE__);

// Core includes
require_once $base_path . '/../includes.inc';

// Settings specific to this script
require_once $base_path . '/transcript_adder/settings.inc';

// Functions specific to tagging
require_once $base_path . '/transcript_adder/transcript_info.inc';

/* Main function to add the transfer credits */
function add_transfer_credits() {
  // Establish Populi API connection
  $populi = new Populi();
  // Use default login credentials
  $populi->login();

  // parse CSV
  // TODO: add something so it gets the filename from the command line?
  $rows = parse_csv($filename);

/* iterate over it */
foreach($rows as $row) {
  /* get ids needed for later steps */
  $person_id = getPersonId($row['firstname'], $row['lastname']);
  $org_id = $populi->searchOrganizations($row['college_name']);
  $program_id = getProgramId($row['program_name']); // Undergraduate or Graduate
  $course_group_id = getCourseGroup($row['course_group_name']); // names from Populi
  $grade_option = getGradeOption($row['grade']);

  // if org_id is null you will need to create a new organization
  // TODO: DUMMY
  if($org_id == NULL) {
    $org_id = 1;
  }

  // create transfer credit item
  $transfer_credit_id = $populi->addTransferCredit($org_id, $person_id, $course_code, $course_name, $num_credits, $course_group_id);

  // add grade
  $result = $populi->addTransferCreditProgram($transfer_credit_id, $program_id, $grade_option);

  // count the successes and failures - see how it was done for tagger
  }
}
/* function to parse the csv - use library? - probably use command line to accept which to do */
function parse_csv($filename) {
  return $rows;
}
