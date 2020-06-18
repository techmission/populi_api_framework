#!/usr/bin/env php
<?php

require_once 'includes.inc';
$populi = new Populi();
$populi->login();
$org_id = $populi->searchOrganizations("City Vision University");
var_dump($org_id);

