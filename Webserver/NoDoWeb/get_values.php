<?php 
//Haal de schakelaar status op
require_once('connections/tc.php');
require_once('include/auth.php');
?>
<?php 
mysql_select_db($database_tc, $tc);
$query_RSvalue = "SELECT id,user_id,display,data,sensor_suffix_false,sensor_suffix_true,graph_type,graph_hours,graph_min_ticksize FROM nodo_tbl_sensor WHERE user_id='$userId'";
$RSvalue = mysql_query($query_RSvalue, $tc) or die(mysql_error());
$row_RSvalue = mysql_fetch_assoc($RSvalue);
//$totalRows_RSvalue = mysql_num_rows($RSvalue);
?>
<?php if ($row_RSvalue != NULL){
do { ?>document.getElementById('value_<?php echo $row_RSvalue['id'];?>').innerHTML = <?php 
   
   if ($row_RSvalue['display'] == 1){
   
		//Lijn grafiek dus laatste meting weergeven
		if ($row_RSvalue['graph_type'] <=1 ){
		
			echo "'".$row_RSvalue['data']."';";
		}
		
		if ($row_RSvalue['graph_type'] == 2){
		
			$sensor_id = $row_RSvalue['id'];
		
			if ($row_RSvalue['graph_hours'] == 0) {
					
					$graph_hours = "24";
				}
				else {
					
					$graph_hours = $row_RSvalue['graph_hours'];
				}
		
		  //Staaf grafiek, totaal meting over gekozen periode weergeven 
		  
		  switch ($row_RSvalue['graph_min_ticksize']) {
		
					case "1":
					$query_RSsensor_value_data = "SELECT id,sensor_id,DATE_FORMAT(timestamp , '%Y-%m-%d %H:%i:%s') as timestamp , ROUND(SUM(data),2) as data FROM nodo_tbl_sensor_data WHERE user_id='$userId' AND sensor_id='$sensor_id' AND timestamp >= SYSDATE() - INTERVAL $graph_hours HOUR GROUP BY minute(timestamp) ORDER BY id DESC LIMIT 1";
					break;
					case "2":
					$query_RSsensor_value_data = "SELECT id,sensor_id,DATE_FORMAT(timestamp , '%Y-%m-%d %H:%i') as timestamp , ROUND(SUM(data),2) as data FROM nodo_tbl_sensor_data WHERE user_id='$userId' AND sensor_id='$sensor_id' AND timestamp >= SYSDATE() - INTERVAL $graph_hours HOUR GROUP BY hour(timestamp) ORDER BY id DESC LIMIT 1";
					break;
					case "3":
					$query_RSsensor_value_data = "SELECT id,sensor_id,DATE_FORMAT(timestamp , '%Y-%m-%d') as timestamp , ROUND(SUM(data),2) as data FROM nodo_tbl_sensor_data WHERE user_id='$userId' AND sensor_id='$sensor_id' AND timestamp >= SYSDATE() - INTERVAL $graph_hours HOUR GROUP BY date(timestamp) ORDER BY id DESC LIMIT 1;";
					break;
					case "4":
					$query_RSsensor_value_data = "SELECT id,sensor_id,DATE_FORMAT(timestamp , '%Y-%m-%d') as timestamp , ROUND(SUM(data),2) as data FROM nodo_tbl_sensor_data WHERE user_id='$userId' AND sensor_id='$sensor_id' AND timestamp >= SYSDATE() - INTERVAL $graph_hours HOUR GROUP BY week(timestamp) ORDER BY id DESC LIMIT 1";
					break;
					case "5":
					$query_RSsensor_value_data = "SELECT id,sensor_id,DATE_FORMAT(timestamp , '%Y-%m') as timestamp , ROUND(SUM(data),2) as data FROM nodo_tbl_sensor_data WHERE user_id='$userId' AND sensor_id='$sensor_id' AND timestamp >= SYSDATE() - INTERVAL $graph_hours HOUR GROUP BY month(timestamp) ORDER BY id DESC LIMIT 1";
					break;
				}
		  
		  	  
		  $RSsensor_value_data = mysql_query($query_RSsensor_value_data, $tc) or die(mysql_error()); 
		  $row_RSsensor_value_data = mysql_fetch_assoc($RSsensor_value_data);
		  //echo "debug..."; 
		  echo "'".$row_RSsensor_value_data['data']."';";
		
		}
		
		
		
		
		
   
   }
   
   if ($row_RSvalue['display'] == 2){
	
		if ($row_RSvalue['data'] <= 0){
		
			echo "'".$row_RSvalue['sensor_suffix_false']."';";
		}
		
		if ($row_RSvalue['data'] > 0){
		
			echo "'".$row_RSvalue['sensor_suffix_true']."';";
		}
		
		
   }
   
   

  
   
 
   
 	
	
	?>
<?php } while ($row_RSvalue = mysql_fetch_assoc($RSvalue)); }?>
<?php
mysql_free_result($RSvalue);

?>