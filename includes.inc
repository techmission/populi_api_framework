<?php

  /* File with all the main includes */
  $base_path = dirname(__FILE__);
  
  /* Connection settings */
  // database connection
  require_once $base_path . '/settings/connection.inc';
  // Populi API settings
  require_once $base_path . '/settings/api_settings.inc';
  
  /* Other constants */
  // logging constants
  require_once $base_path . '/lib/logging/log_constants.inc';
  
  /* Required libraries */
  
  // PDO functions
  require_once $base_path . '/lib/pdo/pdo.inc';
  // Populi functions
  require_once $base_path . '/lib/populi/populi.inc';
  require_once $bath_path . '/lib/populi/populi_methods.inc';
  // Logging functions
  require_once $base_path . '/lib/logging/logging.inc';

  /* Show PHP errors on screen if set to debug */
  // See https://stackoverflow.com/questions/1053424
  if(defined('SCRIPT_MODE') && SCRIPT_MODE == MODE_DEBUG) {
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ALL);
  }
