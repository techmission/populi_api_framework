<?php

/* Wrapper function to create the PDO database */
function db_connect() {
  // Use the constants in the connection string.
  // Throw exceptions on errors for more robust handling.
  // See http://wiki.hashphp.org/PDO_Tutorial_for_MySQL_Developers
  $db = new PDO('mysql:host=' . DB_HOST . ';dbname=' . DB_NAME . ';charset=' . DB_CHARSET, DB_USERNAME, DB_PASSWORD, array(PDO::ATTR_EMULATE_PREPARES => FALSE, PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION));
  return $db;
}
