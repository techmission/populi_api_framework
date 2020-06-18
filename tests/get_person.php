<?php

/* Unit tests for the tagger script */

$base_path = dirname(__FILE__);

// Require core includes
require_once $base_path . '/../includes.inc';

// Not using a real test framework for now, 
// but see https://stackoverflow.com/questions/282150

/* Setup */

$populi = new Populi();
// Use default login credentials
$populi->login();

/* Tag methods */
// See results of get_tags
$people = $populi->getPossibleDuplicatePeopleByName('Evan', 'Donovan');
var_dump($people);
