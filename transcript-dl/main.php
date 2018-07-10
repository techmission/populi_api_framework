<?php

/* Main script that downloads transcripts */

/* Require includes */
$base_path = dirname(__FILE__);

// Core includes
require_once $base_path . '/../includes.inc';

/* Script constants */

// Define a key for security
define('KEY_VALUE', '7303081e-83d7-11e8-8ba0-f23c91e40cf2');

// Get PDF transcript
define('GET_PDF_TRANSCRIPT', TRUE);

/* Download transcript from $_GET parameters */
if (isset($_GET['person_id']) && is_numeric($_GET['person_id']) && isset($_GET['key']) && $_GET['key'] = KEY_VALUE){
    // Establish Populi API connection
    $populi = new Populi();
    // Use default login credentials
    $populi->login();
    // call the wrapper function around Populi's get transcript function to get transcript
	$result = $populi->get_transcript($_GET['person_id'], GET_PDF_TRANSCRIPT);
	
	// Set headers for PDF download
	// https://stackoverflow.com/questions/20080341
	header('Content-Type: application/pdf');
    header('Content-Length: '. strlen( $result ));
    header('Content-disposition: inline; filename="9a.' . $_GET['lastname'] . '_' . $_GET['firstname'] . '_Transcript.pdf"');
    header('Cache-Control: public, must-revalidate, max-age=0');
    header('Pragma: public');
    header('Expires: Sat, 26 Jul 1997 05:00:00 GMT');
    header('Last-Modified: '.gmdate('D, d M Y H:i:s').' GMT');
	// Return the PDF
	print($result);
}
// Log an error if missing parameters
else {
	script_log('Missing parameters needed to download transcript', LEVEL_ERROR);
	exit(-1);
}