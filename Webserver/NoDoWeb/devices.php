<?php 

require_once('connections/tc.php'); 
require_once('include/auth.php'); 
require_once('include/user_settings.php'); 

$page_title = "Devices";



?>

<!DOCTYPE html> 
<html> 

<head>
	
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<title><?php echo $title ?></title> 
	
	<?php require_once('include/jquery_mobile.php'); ?>

	<style type="text/css">	
	
	
	input.ui-slider-input { 
    display:none !important; 
	} 
	
	
	
    .ui-btn-icon-left_collapsible .ui-btn-inner { padding-left: 40px !important;}
	
	.ui-btn-collapsible {
		
		text-align: left !important;
		margin: 0 -8px !important;
		margin-top: 0px !important;
		margin-right: -8px !important;
		margin-bottom: 8px !important;
		margin-left: -8px !important;
		-moz-border-radius: .6em !important;
		-webkit-border-radius: .6em !important;
		border-radius: .6em !important;
				
	}
	
	.ui-btn-collapsible1 {
	
		text-align: left !important;
		margin: 0 -8px !important;
		margin-top: 0px !important;
		margin-right: -8px !important;
			
		margin-left: -8px !important;
		-moz-border-radius: .6em !important;
		-webkit-border-radius: .6em !important;
		border-radius: .6em !important;
		
		-moz-border-radius-bottomleft: 		0em !important;
		-webkit-border-bottom-left-radius: 	0em !important;
		border-bottom-left-radius: 			0em !important;
		
		-moz-border-radius-bottomright: 		0em !important;
		-webkit-border-bottom-right-radius: 	0em !important;
		border-bottom-right-radius: 			0em !important;
	
	}
	
	
	</style>
	
	<!-- Get device state -->
	<script src="js/get_device_state.js"></script>
	<!-- /Get device state -->

	<?php require_once('include/send_event.php'); ?>
	
</head> 

<body> 

<div data-role="page" data-theme="<?php echo $theme?>" data-title="<?php echo $title ?>">

<?php require_once('include/header.php'); ?>
	<div data-role="content">	
	  
	
<?php 

mysql_select_db($database_tc, $tc);
$query_RSdevices = "SELECT * FROM nodo_tbl_devices WHERE user_id='$userId' ORDER BY sort_order ASC";
$RSdevices = mysql_query($query_RSdevices, $tc) or die(mysql_error());


while ($row_RSdevices = mysql_fetch_assoc($RSdevices)){ 

$id = $row_RSdevices['id']; 

//Toggles
if ($row_RSdevices['toggle'] == 1) {
	
	
	switch ($row_RSdevices['type']) {
				
		case 1: //KAKU
		case 3: //WiredOut
		case 4: //UserEvents
		echo "<a href=\"javascript:send_event(&quot;&action=toggle&device_id=". $id . "&quot;)\" class=\"ui-btn-collapsible ui-btn-icon-left_collapsible\" data-role=\"button\" data-shadow=\"false\" data-icon=\"star\" ><span id='switch_" . $row_RSdevices['id'] . "'></span>" . $row_RSdevices['naam'] . "</a>\n";
		break;
		
		case 2: //NewKaku
		//Dim buttons		
		if ( $row_RSdevices['dim'] == 1 ) { 
			
			// newkaku toggle button
			echo "<a href=\"javascript:send_event(&quot;&action=toggle&device_id=". $id . "&quot;)\" class=\"ui-btn-collapsible1 ui-btn-icon-left_collapsible \" data-role=\"button\" data-shadow=\"false\" data-icon=\"star\"  ><span id='switch_" . $row_RSdevices['id'] . "'></span>" . $row_RSdevices['naam'] . "</a>\n";
			echo "<div class=\"ui-collapsible-content ui-body-" . $theme . " ui-corner-bottom\">";
			
			for ($i=1; $i<=10; $i++) {
			
				   $dim_percentage = $i * 10;
				   $dim_level = round($i * 1.6,0);
				   
					echo "<a href=\"javascript:send_event(&quot;&action=dim&device_id=". $id . "&dim=" .$dim_level. "&quot;)\" data-role=\"button\" data-mini=\"true\" data-inline=\"true\">".$dim_percentage."%</a>\n";
			}
			
			echo "</div> <div style=\"padding: 0px 0px 8px 0px;\"></div>";
		}
					
			
		//Dim slider
		elseif ( $row_RSdevices['dim'] == 2 ) { 
		
			echo "<a href=\"javascript:send_event(&quot;&action=toggle&device_id=". $id . "&quot;)\" class=\"ui-btn-collapsible1 ui-btn-icon-left_collapsible \" data-role=\"button\" data-shadow=\"false\" data-icon=\"star\"  ><span id='switch_" . $row_RSdevices['id'] . "'></span>" . $row_RSdevices['naam'] . "</a>\n";
						
			echo "<div class=\"ui-collapsible-content ui-body-" . $theme . " ui-corner-bottom\">";
			echo "<script>\n";
				echo "var t".$row_RSdevices['id'].";\n";
				echo "function update_distance_timer_".$row_RSdevices['id']."()\n";
					echo "{\n";
					echo "clearTimeout(t".$row_RSdevices['id'].");\n";
					echo "t".$row_RSdevices['id']."=setTimeout(\"update_distance_".$row_RSdevices['id']."()\",200);\n";
					echo "}\n";
				echo "function update_distance_".$row_RSdevices['id']."()\n";
					echo "{\n";
					echo "var val".$row_RSdevices['id']." = $('#distSlider".$row_RSdevices['id']. "').val();\n";
					echo "send_event('&action=dim&device_id=". $id . "&dim=' + val".$row_RSdevices['id'].")\n";
					echo "}\n";
			echo "</script>\n";			
			
			echo "<label id=\"distSlider-label\" for=\"distSlider\">Dim: </label>\n";
			echo "<input name=\"distSlider\" id=\"distSlider".$row_RSdevices['id']."\" value=\"".$row_RSdevices['dim_value']."\" min=\"1\" max=\"16\" data-type=\"range\" onChange='update_distance_timer_".$row_RSdevices['id']."()'>\n";
			
			
			echo "</div> <div style=\"padding: 0px 0px 8px 0px;\"></div>";
		 }
		else {
			// newkaku toggle button
			echo "<a href=\"javascript:send_event(&quot;&action=toggle&device_id=". $id . "&quot;)\" class=\"ui-btn-collapsible ui-btn-icon-left_collapsible\" data-role=\"button\" data-shadow=\"false\" data-icon=\"star\"  ><span id='switch_" . $row_RSdevices['id'] . "'></span>" . $row_RSdevices['naam'] . "</a>\n";
		}
		
		break;
		
		
		
	}

	
}

else {

?>

	<div data-role="collapsible" data-content-theme="<?php echo $theme?>" <?php if($row_RSdevices['collapsed']==1) {echo "data-collapsed=\"false\"";}?> >
	<h3><span id='switch_<?php echo $row_RSdevices['id']; ?>'></span><?php echo $row_RSdevices['naam']; ?></h3>
	<p>
			  
	<?php 


	switch ($row_RSdevices['type'])
				{
				case "1":
				// On/Off kaku buttons
				if ($row_RSdevices['label_on'] != "") {$label_on = $row_RSdevices['label_on'];} else {$label_on = "On";}
				if ($row_RSdevices['label_off'] != "") {$label_off = $row_RSdevices['label_off'];} else {$label_off = "Off";}

				
				
				echo "<a href=\"javascript:send_event(&quot;&action=on&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"check\"  >" . $label_on . "</a>\n";
				echo "<a href=\"javascript:send_event(&quot;&action=off&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"delete\" >" . $label_off . "</a>\n";
									
				break;  
							
				case "2":
				// On/Off newkaku buttons 
				if ($row_RSdevices['label_on'] != "") {$label_on = $row_RSdevices['label_on'];} else {$label_on = "On";}
				if ($row_RSdevices['label_off'] != "") {$label_off = $row_RSdevices['label_off'];} else {$label_off = "Off";}
				echo "<a href=\"javascript:send_event(&quot;&action=on&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"check\" >" . $label_on . "</a>\n";
				echo "<a href=\"javascript:send_event(&quot;&action=off&device_id=". $id . "&quot;)\" data-role=\"button\"  data-icon=\"delete\" >" . $label_off . "</a>\n";
				echo "<br>\n";
						
					//Dim buttons		
					if ( $row_RSdevices['dim'] == 1 ) { 
						
						for ($i=1; $i<=10; $i++) {
						
							   $dim_percentage = $i * 10;
							   $dim_level = round($i * 1.6,0);
							   
								echo "<a href=\"javascript:send_event(&quot;&action=dim&device_id=". $id . "&dim=" .$dim_level. "&quot;)\" data-role=\"button\" data-inline=\"true\">".$dim_percentage."%</a>\n";
						}
					}		   
			
					//Dim slider
					if ( $row_RSdevices['dim'] == 2 ) { 
									
						
						echo "<script>\n";
							echo "var t".$row_RSdevices['id'].";\n";
							echo "function update_distance_timer_".$row_RSdevices['id']."()\n";
								echo "{\n";
								echo "clearTimeout(t".$row_RSdevices['id'].");\n";
								echo "t".$row_RSdevices['id']."=setTimeout(\"update_distance_".$row_RSdevices['id']."()\",200);\n";
								echo "}\n";
							echo "function update_distance_".$row_RSdevices['id']."()\n";
								echo "{\n";
								echo "var val".$row_RSdevices['id']." = $('#distSlider".$row_RSdevices['id']. "').val();\n";
								echo "send_event('&action=dim&device_id=". $id . "&dim=' + val".$row_RSdevices['id'].")\n";
								echo "}\n";
						echo "</script>\n";			
						
						echo "<label id=\"distSlider-label\" for=\"distSlider\">Dim: </label>\n";
						echo "<input name=\"distSlider\" id=\"distSlider".$row_RSdevices['id']."\" value=\"".$row_RSdevices['dim_value']."\" min=\"1\" max=\"16\" data-type=\"range\" onChange='update_distance_timer_".$row_RSdevices['id']."()'>\n";
						
						

					 } 
				break;    
				
				case 3:
				// On/Off WiredOut buttons
				if ($row_RSdevices['label_on'] != "") {$label_on = $row_RSdevices['label_on'];} else {$label_on = "On";}
				if ($row_RSdevices['label_off'] != "") {$label_off = $row_RSdevices['label_off'];} else {$label_off = "Off";}
				echo "<a href=\"javascript:send_event(&quot;&action=on&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"check\" >" . $label_on . "</a>\n";
				echo "<a href=\"javascript:send_event(&quot;&action=off&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"delete\" >" . $label_off . "</a>\n";
				
				break;
				 
				 case 4:
				// On/Off Userevents
				if ($row_RSdevices['label_on'] != "") {$label_on = $row_RSdevices['label_on'];} else {$label_on = "On";}
				if ($row_RSdevices['label_off'] != "") {$label_off = $row_RSdevices['label_off'];} else {$label_off = "Off";}
				echo "<a href=\"javascript:send_event(&quot;&action=on&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"check\" >" . $label_on . "</a>\n";
				echo "<a href=\"javascript:send_event(&quot;&action=off&device_id=". $id . "&quot;)\" data-role=\"button\" data-icon=\"check\" >" . $label_off . "</a>\n";
				
				break;
				}
		
				echo "</p></div>";
		 } 
}?>
	 
	

<script>
//Device status loop eerste keer opstarten
Device_State();
</script>	
	 
	 
	</div><!-- /content -->
	

	
	<?php require_once('include/footer.php'); ?>
	
	
</div><!-- /page -->


</body>
</html>
