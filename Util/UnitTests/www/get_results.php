<?php
require('DB/mysqli.php');

$db = mysqli_connect("localhost", "ResultReader", "results", "testserver");
if(mysqli_connect_errno($db)) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}


$query = "
  (        SELECT performance_results.id as id, test_runs.timestamp, server_id, server_name, test_run_id, file_id, filename, case_id, test_type, case_name,
                null as success, null as output_image, null as diff_image, null as baseline_image, null as step_name, min_fps, max_fps, mean_fps, avg_fps
          FROM performance_results
            JOIN test_runs ON performance_results.test_run_id=test_runs.id
            JOIN test_cases ON performance_results.case_id=test_cases.id
            JOIN test_files ON performance_results.file_id=test_files.id
            JOIN servers ON test_runs.server_id=servers.id
            JOIN (SELECT MAX(perf_inner.id) as id, MAX(timestamp) as maxtime FROM performance_results as perf_inner, test_runs WHERE perf_inner.test_run_id=test_runs.id GROUP BY case_id ORDER BY timestamp DESC) as perf_outer 
                  ON perf_outer.id = performance_results.id AND perf_outer.maxtime = test_runs.timestamp
) UNION ALL (
            SELECT rendering_results.id as id, test_runs.timestamp, server_id, server_name, rendering_results.test_run_id, rendering_results.file_id, filename, rendering_results.case_id, test_type, case_name,
         success, rendering_results.output_image, rendering_results.diff_image, image AS baseline_image, baseline.step_name, null as min_fps, null as max_fps, null as mean_fps, null as avg_fps
  FROM rendering_results
    JOIN test_runs ON rendering_results.test_run_id=test_runs.id
    JOIN test_cases ON rendering_results.case_id=test_cases.id
    JOIN test_files ON rendering_results.file_id=test_files.id
    JOIN servers ON test_runs.server_id=servers.id
    LEFT JOIN rendering_baselines as baseline ON rendering_results.case_id=baseline.case_id AND rendering_results.step_name=baseline.step_name
    JOIN (SELECT MAX(rend_inner.id) as id, MAX(timestamp) as maxtime, case_id FROM rendering_results as rend_inner JOIN test_runs ON rend_inner.test_run_id=test_runs.id GROUP BY case_id, file_id, step_name ORDER BY timestamp DESC) as rend_outer
            ON rend_outer.id = rendering_results.id AND rend_outer.maxtime = test_runs.timestamp
            GROUP BY case_id, file_id, step_name)";

$data = generate_results($db, $query);

// After this is done we've successfully built our object and just need to convert it to json.
  echo json_encode($data, JSON_PRETTY_PRINT);
	
function generate_results($db, $query){	
if(!$fetch_result = mysqli_query($db, $query)) {
		die("ERROR: " . mysql_error());
	}	

 
  /*
    Tree-building algorithm:

    for every row:
      loop through the $data array and look for an element with a name property that matches the first name in the filename path.
      if it doesn't exist
        create and add it to data (create with a children array if it isn't the last part of the filename path, otherwise with a testcases array)
        point $node at the newly added node
      else
        point $node at it
        
      if there is more than one element in the filename path
        remove the first element of the filename path
      
      while there's more than 1 parts left of the filename path
        if the current node has a child with a name that matches the first part of the filename path
          point $node at that child node
        else
          create a new node, same as before, and add it to that children array
          point $node at the new node
        remove the first element of the filename path
      
      if node contains a children array
        if node children contains an element for this testcase
          point $node at that element
        else
          create an element for this testcase
          point $node at that element
      else
        //if we get here then that means this is a top-level leaf and we can just proceed to the end of the code
      
      add the data to $node['testcases']
        
  */									

  $data = array();
  while($row = mysqli_fetch_assoc($fetch_result)) {
  //  echo json_encode($row);
    $filename = $row['filename'];
    $category_structure = explode('/', $filename);
    // Store a reference to the root of the data tree. We'll move this reference down the tree until we reach the part where the leaf should go,
    // creating any missing nodes on the way there.
    // NOTE: json_encode won't output an array if there are any named elements in it. It'll turn those into objects.
    $node = &$data;

    $index = -1;
    for($i = 0; $i < count($data); $i++) {
      if(strcmp($data[$i]['name'], $category_structure[0]) == 0) {
        $index = $i;
        break;
      }
    }
    if($index == -1) { // We don't have a top-level entry for this category or testcase, so let's add it.
      if(count($category_structure) > 1) {
        $node = &$data[array_push($data, array("name" => $category_structure[0], "children" => array()))-1];
      } else {
        $node = &$data[array_push($data, array("name" => $category_structure[0], "testcases" => array()))-1];
      }
    } else {
      $node = &$data[$index];
    }
    if(count($category_structure) > 1)
      array_shift($category_structure);
  //  echo '</br>it is now ' . json_encode($category_structure) ."</br>";
          
    // Go through every folder specified in the casename path and build our array structure.
    while(count($category_structure) > 1) {
      $index = -1;
      for($i = 0; $i < count($node['children']); $i++) {
        if((strcmp($node['children'][$i]['name'], $category_structure[0]) == 0)) {
          $index = $i;
          break;
        }      
      }
      if($index > -1) {
        $node = &$node['children'][$i];
      } else {     
        $new_node = array("name" => $category_structure[0], "children" => array());
        $node = &$node['children'][array_push($node['children'], $new_node)-1];
  //  echo "node is: </br>" . json_encode($node) . "</br>";
      }
      // Remove the first element from $category_structure
      array_shift($category_structure); 
    }
    //if node contains a children array then it is the category in which the testcase leaf should be
    if(array_key_exists('children', $node)) {
      
      $index = -1;
      for($i = 0; $i < count($node['children']); $i++) {
        if((strcmp($node['children'][$i]['name'], $category_structure[0]) == 0)) {
          $index = $i;
          break;
        }      
      }
      //if node children contains an element for this testfile
      if($index > -1) {
        //point $node at that element
        $node = &$node['children'][$index];
      } else {
        //create and push an element for this testfile into the array
        //point $node at that element
        $new_node = array( 'name' => $category_structure[0], 'testcases' => array());
        $node = &$node['children'][array_push($node['children'], $new_node)-1];
        }
    }

    $testcase = array(
      "name"   => $row['case_name'],
      "filename"   => $row['filename'],
      "test_type"   => $row['test_type'],
      "test_run_id" => $row['test_run_id'],
      "server_id"=> $row['server_id'],
      "server_name"=> $row['server_name'],
      "time"   => $row['timestamp']);
    
    if($row['test_type'] == "rendering") {
      $testcase["success"] = $row['success'];
      $testcase["step_name"] = $row['step_name'];
      $testcase["output_image"] = base64_encode($row['output_image']);
      $testcase["diff_image"] = base64_encode($row['diff_image']);
      $testcase["baseline_image"] = base64_encode($row['baseline_image']);
    }
    else if($row['test_type'] == "performance") {
      $testcase["min_fps"] = $row['min_fps'];
      $testcase["avg_fps"] = $row['avg_fps'];
      $testcase["mean_fps"] = $row['mean_fps'];
      $testcase["max_fps"] = $row['max_fps'];
    $testcase['history'] = array();
    // Now build the history array
    $query = "SELECT min_fps, avg_fps, max_fps, mean_fps, timestamp, server_id, server_name, test_run_id 
                                  FROM performance_results 
                                    JOIN test_runs
                                      ON performance_results.test_run_id=test_runs.id
                                    JOIN servers
                                      ON test_runs.server_id=servers.id
                                    WHERE case_id=" .$row['case_id']. " AND timestamp<=\"" .$row['timestamp'] . "\" AND test_run_id!=".$row['test_run_id']." ORDER BY timestamp ASC";
  //  echo "<br><br>Query: ".$query;
    $history_fetch = mysqli_query($db, $query);
    while($hist_row = mysqli_fetch_assoc($history_fetch)) {
       array_push($testcase['history'], array(
      "test_run_id" => $hist_row['test_run_id'],
      "server_id"=> $hist_row['server_id'],
      "server_name"=> $hist_row['server_name'],
      "time"   => $hist_row['timestamp'],
      "min_fps"=> $hist_row['min_fps'],
      "avg_fps"=> $hist_row['avg_fps'],
      "mean_fps"=>$hist_row['mean_fps'],
      "max_fps"=> $hist_row['max_fps'],
       ));
       }
    }
    

    // All that remains now is to push the testcase to the node's testcases array
    array_push($node['testcases'], $testcase);
  }
  return $data;
}



?>