<?php

// Get the date for a particular term, by key
function get_term_date($term_name) {
  $term_dates = get_term_dates();
  $term_date = $term_dates[$term_name];
  // Handle the case in which the term name is not matched
  if($term_date == '' || $term_date == NULL) {
    $term_date = '';
  }
  return $term_date;
}

// List all the dates for terms
function get_term_dates() {
  $term_dates = array(
    'Summer 2010' => 'May 31 - Jul. 24',
    'Fall 1 2010' => 'Aug. 23 - Oct. 15',
    'Fall 2 2010' => 'Oct. 25 - Dec. 17',
    'Spring 1 2011' => 'Jan. 3 - Mar. 7',
    'Spring 2 2011' => 'Mar. 14 - May 13',
    'Summer 2011' => 'Jun. 4 - July 29',
    'Fall 1 2011' => 'Aug. 22 - Oct. 14',
    'Fall 2 2011' => 'Oct. 24 - Dec. 19',
    'Spring 1 2012' => 'Jan. 2 - Feb. 23',
    'Spring 2 2012' => 'Mar. 12 - May 11',
    'Summer 2012' => 'Jun. 4 - Jul. 29',
    'Fall 1 2012' => 'Aug. 27 - Oct. 21',
    'Fall 2 2012' => 'Oct. 29 - Dec. 23',
    'Spring 1 2013' => 'Jan. 7 - Mar. 3',
    'Spring 2 2013' => 'Mar. 18 - May 12',
    'Summer 2013' => 'May 27 - Jul. 21',
    'Fall 1 2013' => 'Aug. 26 - Oct. 21',
    'Fall 2 2013' => 'Oct. 28 - Dec. 22',
    'Spring 1 2014' => 'Jan. 6 - Mar. 2',
    'Spring 2 2014' => 'Mar. 17 - May 11',
    'Summer 2014' => 'May 26 - Jul. 20',
    'Fall 1 2014' => 'Aug. 25 - Oct. 19',
    'Fall 2 2014' => 'Oct. 27 - Dec. 27',
    'Spring 1 2015' => 'Jan. 5 - Mar. 1',
    'Spring 2 2015' => 'Mar. 16 - May 10',
    'Summer 2015' => 'May 25 - Jul. 19',
    'Fall 1 2015' => 'Aug. 24 - Oct. 18',
    'Fall 2 2015' => 'Oct. 26 - Dec. 20',
    'Spring 1 2016' => 'Jan. 4 - Feb. 29',
    'Spring 2 2016' => 'Mar. 14  - May 9',
    'Summer 2016' => 'May 23 - Jul. 18',
    'Fall 1 2016' => 'Aug. 22 - Oct. 17',
    'Fall 2 2016' => 'Oct. 24 - Dec. 19',
    'Spring 1 2017' => 'Jan. 9 - Mar. 6',
    'Spring 2 2017' => 'Mar. 20 - May 15',
    'Summer 2017' => 'Jun. 5 - Jul. 31',
    'Fall 1 2017' => 'Aug. 21 - Oct. 16',
    'Fall 2 2017' => 'Oct. 23 - Dec. 31',
    'Spring 1 2018' => 'Jan. 8 - Mar. 5',
    'Spring 2 2018' => 'Mar. 19 - May 13',
    'Summer 2018' => 'Jun. 4 - Jul. 29',
  );
  return $term_dates;
}
