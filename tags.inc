<?php

// Return the tags array (dummied for now)
function get_tags() {
  return array(
    'tagname' => 1,
  );
}

function get_tag_id_by_name($name) {
  $tags = get_tags();
  return $tags[$name];
}

