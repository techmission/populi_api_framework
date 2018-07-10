<?php

/* Bootstrap the codebase */

chdir(dirname(__FILE__));

require_once('controller.inc'); // main business logic
transcript_bootstrap();         // add rest of code

// Build the transcript data array; uses data from $_GET
$transcript_data = build_transcript_data();

// Render using Twig.
// See https://twig.sensiolabs.org/doc/2.x/api.html#twig-for-developers
global $TWIG;
// Debugging vars
global $VARS;
if(isset($_GET['Format'])) {
  $format = $_GET['Format'];
}
$template_file = 'transcript.html';
if($format == FORMAT_SIMPLE) {
  $template_file = 'transcript-simple.html';
}
$template = $TWIG->load($template_file);
echo $template->render(array('t' => $transcript_data, 'vars' => $VARS));
?>
