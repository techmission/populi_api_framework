<?php

/* File to define constants used in the codebase. */

/* Debug logging levels */
define('LOG_ALL', 0);    // log_to_screen()
define('LOG_SOME', 1);   // templates
define('LOG_ERRORS', 2); // errors only (production)
define('LOG_NONE', 3);   // silence

/* Query types */
define('QUERY_STUDENTNAME', 0);
define('QUERY_EMAIL', 1);

/* Error messages */
define('ERROR_CONTACT_NOT_FOUND', 'A contact was not found based on the provided information.');
define('ERROR_INVALID_INPUT', 'The provided input could not be used to query Salesforce.');
define('ERROR_INVALID_TRANSCRIPT', 'The transcript data was in an incorrect format to render.');

/* Salesforce record types */

// define('RECORD_TYPE_STUDENT', '012A0000000Ntpn');
// define('RECORD_TYPE_COURSE', '012A0000000smdb');

/* Types of SAP block */
define('STATUS_MAKING_SAP', 'Making SAP');
define('STATUS_WARNING', 'Warning');
define('STATUS_BLOCKED_PENDING_APPEAL', 'Blocked from Aid Pending Appeal');
define('STATUS_BLOCKED', 'Blocked from Aid');
// Will not be used for now (no data to look up)
define('STATUS_PROBATION', 'Probation');

// number of credits default
define('CREDITS_DEFAULT', 3);

// transcript templates
define('FORMAT_SAP', 0);
define('FORMAT_SIMPLE', 1);
