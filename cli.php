#!/usr/bin/env php
<?php
// adapted from https://github.com/will-garrett/populiAPI-CLI

// example: 
// ./cli.php addTransferCreditProgram transfer_credit_id 6377246 program_id 25213 grade 82 pass_fail FALSE

require_once 'includes.inc';

// parse command line
$num_args=sizeof($argv)-1;
$task=$argv[1];

$populi = new Populi();
$populi->login();

$params = array();
for($i=2; $i<=$num_args; $i++){
   if($i%2 == 0){
     $params[$argv[$i]]=$argv[$i+1];
   }
}
var_dump($params);

$result = $populi->doTask($task,$params);
var_dump($result);

// TODO: implement the logout method
//$populi->logout();
