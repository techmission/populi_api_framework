<?php

/* Unit tests for the tagger script */

$base_path = dirname(__FILE__);

// Require core includes
require_once $base_path . '/../includes.inc';

// Require the settings and additional methods for the tagger script
require_once $base_path . '/tagger/settings.inc';

// Not using a real test framework for now, 
// but see https://stackoverflow.com/questions/282150

script_log('Test 1', LEVEL_ERROR);
script_log('Test 2', LEVEL_DEBUG);
