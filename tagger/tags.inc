<?php

// Return the tags array (dummied for now)
function get_tags() {
  // Key / value array of names and id's matching Populi
  return array(
    'Pell 16-17' => 269497,
    'Loan 16-17' => 269587,
    '17-18 ISIR' => 272209,
    'Completed VFAO Interview' => 272533,
  );
}

function get_tag_id_by_name($name) {
  $tags = get_tags();
  // Look up a tag in the array
  $tag_id = $tags[$name];
  if(is_numeric($tag_id)) {
    return $tag_id;
  }
  // If there is no match, return FALSE
  else {
    return FALSE;
  }
}

