<?php

/* Main script that downloads transcripts */

/* Require includes */
$base_path = dirname(__FILE__);

// Core includes
require_once $base_path . '/../includes.inc';

/* Script settings - change for your Populi instance */

// Program ID for transcript:
// Undergraduate program (in programs table)
define('PROGRAM_ID', 25207);

// Layout name for transcript:
// Use getPrintLayouts method to find out a layout id
// or select * from files where type = "PAGE_LAYOUT"
define('TRANSCRIPT_LAYOUT_NAME', 'USDE Transcript');

/* Download transcript from $_GET parameters */
if (isset($_GET['person_id']) && is_numeric($_GET['person_id']) && isset($_GET['key']) && $_GET['key'] = GUID){
    // Establish Populi API connection
    $populi = new Populi();
    // Use default login credentials
    $populi->login();
	
	// get the Populi print layout by name
	$result = $populi->doTask('getPrintLayouts');
	// iterate over the returned results
	foreach($result->print_layout as $layout) {
		if($layout->type == 'TRANSCRIPT' && $layout->name == TRANSCRIPT_LAYOUT_NAME) {
			$layout_id = $layout->id;
			break;
		}
	}
    // call the wrapper function around Populi's get transcript function to get transcript
	$params = array('person_id' => $_GET['person_id'], 'pdf' => TRUE, 'layout_id' => $layout_id, 'program_id' => PROGRAM_ID, 'official' => TRUE);
	$result = $populi->get_transcript($params);
	
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
