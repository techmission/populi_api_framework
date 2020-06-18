<?php

// Change this constant when you wish to change the script's log level
// and execution mode.
define('SCRIPT_MODE', MODE_DEBUG);

// Full path to log file
// Revise later to be just the filename (once generalized logger working)
define('LOGFILE', '/var/log/populi_tagger.log');

  /* Show PHP errors on screen if set to debug */
  // See https://stackoverflow.com/questions/1053424
  if(SCRIPT_MODE == MODE_DEBUG) {
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ALL);
  }
