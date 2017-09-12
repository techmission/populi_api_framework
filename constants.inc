<?php

// Script modes
define('MODE_DEBUG', 1);  // debugging lines - break execution at breakpoints
define('MODE_DEFAULT', 0); // no debugging lines

// Log levels for messages
define('LEVEL_DEBUG', 0);
define('LEVEL_ERROR', 1);

// Current time (for logging)
define('CURTIME', date('YmdHi'));
define('LOGFILE', 'populi_tagger.log');
