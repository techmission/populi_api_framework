<?php

/* Unit tests for the tagger script */

$base_path = dirname(__FILE__);

// Require core includes
require_once $base_path . '/../includes.inc';

// Require the settings and additional methods for the tagger script
require_once $base_path . '/tagger/settings.inc';
require_once $base_path . '/tagger/tags.inc';

// Not using a real test framework for now, 
// but see https://stackoverflow.com/questions/282150

/* Setup */
$db = db_connect();

$populi = new Populi();
// Use default login credentials
$populi->login();

/* Tag methods */
// See results of get_tags
echo 'Get tags: ' . PHP_EOL;
$tags = get_tags();
var_dump($tags);
echo PHP_EOL;

// Set results of get_tag_by_name
echo 'Get tag id by name: ' . PHP_EOL;
$tag_id = get_tag_id_by_name('Pell 16-17'));
var_dump($tag_id);
echo PHP_EOL;

* Populi methods */
// this is confirmed in base case
echo 'possibleDuplicatePeopleByName' . PHP_EOL;
$students = $populi->getPossibleDuplicatePeopleByName('Evan', 'Donovan');
var_dump($students);

// student id of first one
echo 'Student id ' . PHP_EOL;
$student_id = $students[0]['id'];
var_dump($student_id);
echo PHP_EOL;

// addTag
$result = $populi->addTag($student_id, $tag_id);
var_dump($result);

// Do removeTag after I check on Populi itself
