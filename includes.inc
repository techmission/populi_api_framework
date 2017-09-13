<?php

  /* File with all the main includes */
  $base_path = dirname(__FILE__);
  
  /* Connection settings */
  // database connection
  require_once $base_path . 'connection.inc';
  // Populi API settings
  require_once $base_path . 'api_settings.inc';
  
  /* Other constants */
  require_once $base_path . 'constants.inc';
  
  /* Settings that can change */
  // Tag names
  require_once $base_path . 'tags.inc';
  // Other settings (such as debug mode)
  require_once $base_path . 'settings.inc';
  
  /* Required libraries */
  
  // PDO functions
  require_once $base_path . 'pdo.inc';
  // Populi functions
  require_once $base_path . 'populi.inc';
  // Logging functions
  require_once $base_path . 'logging.inc';
