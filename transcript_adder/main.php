<?php

/* Main script that does the tagging */

/* Require includes */
$base_path = dirname(__FILE__);

// Core includes
require_once $base_path . '/../includes.inc';

// Settings specific to this script
require_once $base_path . '/transcript_adder/settings.inc';

// Functions specific to tagging
require_once $base_path . '/transcript_adder/transcript_info.inc';

function add_transfer_credits() {
  // Establish Populi API connection
  $populi = new Populi();
  // Use default login credentials
  $populi->login();
}
