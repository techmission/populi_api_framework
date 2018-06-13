<?php

/* File with the main business logic that builds the transcript page and the error page */

/* Includes all the rest of the needed files. */
function transcript_bootstrap() {
  global $TWIG;
  // Add Twig.
  require_once('vendor/autoload.php');
  $loader = new Twig_Loader_Filesystem('templates');
  // Added auto_reload for debugging
  $TWIG = new Twig_Environment($loader, array('cache' => 'twig_cache', 'auto_reload' => TRUE));
  
  // Add includes for this codebase
  require_once('../includes.inc')       // Populi API integration framework
  require_once('lib.inc');              // utility functions (maybe unneeded)
  require_once('constants.inc');        // defines constants
  require_once('settings.inc');         // defines settings that can change
  require_once('term_dates.inc');       // defines dates of terms
  require_once('queries.inc');          // wrapper functions for querying
  require_once('process.inc');          // processes the transcript data

  // Connect to Populi
  global $POPULI;
  $POPULI = new Populi();
  // Use default login credentials
  $POPULI->login();
}

/* Function to build the transcript page */
function build_transcript_data() {
  $transcript_data = array();
  $student_id = '';
  $output = '';
  if(!empty($_GET['FirstName'] && $_GET['LastName'])) {
    $student_id = get_student_id(QUERY_STUDENTNAME, array('FirstName' => $_GET['FirstName'], 'LastName' => $_GET['LastName']));
   if(!is_populi_id($student_id)) {
     build_error_page(ERROR_CONTACT_NOT_FOUND, array('FirstName' => $_GET['FirstName'], 'LastName' => $_GET['LastName'], 'student_id' => $student_id));
   }
  }
  else if(!empty($_GET['Email'])) {
    $student_id = get_student_id(QUERY_EMAIL, array('Email' => $_GET['Email']));
    if(!is_populi_id($student_id)) {
      build_error_page(ERROR_CONTACT_NOT_FOUND, array('Email' => $_GET['Email'], 'student_id' => $student_id));
    }
  }  
  else if(!empty($_GET['Student_Id']) && is_sfid($_GET['Student_Id'])) {
    $student_id = $_GET['Id'];
  }
  else {
    build_error_page(ERROR_INVALID_INPUT, array('_GET' => $_GET));
  }
  
  $transcript_data = get_transcript_data($student_id);
  if(is_transcript_valid($transcript_data)) {
    return process_transcript_data($transcript_data);
  }
  else {
    build_error_page(ERROR_INVALID_TRANSCRIPT, array('transcript_data' => $transcript_data));
  }
}

/* Function to build the error page */
function build_error_page($error_msg, $vars) {
  global $TWIG;
  $template = $TWIG->load('error.html');
  $vars_dump = print_r($vars, TRUE);
  echo $template->render(array('error_msg' => $error_msg, 'vars' => $vars, 'vars_dump' => $vars_dump, 'log_mode' => LOG_MODE));
  // TODO: add the logging from the Populi framework here
  // needs to be adjusted to be configurable in which log file it uses
  exit(-1);
}
