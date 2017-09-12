<?php

// Return the tags array (dummied for now)
function get_tags() {
  return array(
    'Pell 16-17' => 269497,
    'Loan 16-17' => 269587,
    '17-18 ISIR' => 272209,
    'Completed VFAO Interview' => 272533,
  );
}

function get_tag_id_by_name($name) {
  $tags = get_tags();
  return $tags[$name];
}

