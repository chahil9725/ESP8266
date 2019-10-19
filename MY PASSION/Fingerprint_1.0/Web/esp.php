<?php




?>
<html>
<head>
	<title>Displaying MySQL Data in HTML Table</title>
	<style type="text/css">
		body {
			font-size: 15px;
			color: #343d44;
			font-family: "segoe-ui", "open-sans", tahoma, arial;
			padding: 0;
			margin: 0;
		}
		table {
			margin: auto;
			font-family: "Lucida Sans Unicode", "Lucida Grande", "Segoe Ui";
			font-size: 12px;
		}

		h1 {
			margin: 25px auto 0;
			text-align: center;
			text-transform: uppercase;
			font-size: 17px;
		}

		table td {
			transition: all .5s;
		}
		
		/* Table */
		.data-table {
			border-collapse: collapse;
			font-size: 14px;
			min-width: 537px;
		}

		.data-table th, 
		.data-table td {
			border: 1px solid #e1edff;
			padding: 7px 17px;
		}
		.data-table caption {
			margin: 7px;
		}

		/* Table Header */
		.data-table thead th {
			background-color: #508abb;
			color: #FFFFFF;
			border-color: #6ea1cc !important;
			text-transform: uppercase;
		}

		/* Table Body */
		.data-table tbody td {
			color: #353535;
		}
		.data-table tbody td:first-child,
		.data-table tbody td:nth-child(4),
		.data-table tbody td:last-child {
			text-align: right;
		}

		.data-table tbody tr:nth-child(odd) td {
			background-color: #f4fbff;
		}
		.data-table tbody tr:hover td {
			background-color: #ffffa2;
			border-color: #ffff0f;
		}

		/* Table Footer */
		.data-table tfoot th {
			background-color: #e5f5ff;
			text-align: right;
		}
		.data-table tfoot th:first-child {
			text-align: left;
		}
		.data-table tbody td:empty
		{
			background-color: #ffcccc;
		}
	</style>
</head>
<body>
	<h1>Table 1</h1>
	<table class="data-table">
		<caption class="title">Student Fingerprint Database</caption>
		<thead>
			<tr>
				<th>ENROLLMENT</th>
				<th>NAME</th>
				<th>ID</th>
				<th>TIME</th>
				<th>DATE</th>
			</tr>
		</thead>
		<tbody>
		<?php
       
          
          
$servername = "www.db4free.net";
$username = "chahilpatel";
$password = "8866551417";
$dbname = "esp8266";
//$name = $_GET['name'];
//$enroll= $_GET['enroll'];
//$id = $_GET['id'];
//$time = $_GET['time'];
//$date = $_GET['date'];

// Insert these values if none have been passed

//echo("Name= ".$name."    "."Enrollment = ".$enroll."   "."ID = ".$id."   "."Time = ".$time."   "."Date = ".$date."<BR>");

// Connect to mysql server
$conn = @mysql_pconnect($servername,$username,$password ) 
  or die("Connection failed! ".mysql_error());

// Select database
mysql_select_db($dbname,$conn) 
  or die ("Database selection failed! ".mysql_error());  

$now = new DateTime();  
$datenow = $now->format("Y-m-d H:i:s");

$sql = ("SELECT * FROM studentdata");
$query = mysqli_query($sql,$conn);
  //or die("Invalid query: ".mysql_error());

//echo "<h2>The Field and Value data have been sent</h2>";




          
         
         
			while ($row = mysqli_fetch_array($query,mysqli_result))
		{
		
			echo '<tr>
					<td>'.$row['enroll'].'</td>
					<td>'.$row['name'].'</td>
					<td>'.$row['id'].'</td>
					<td>'.$row['time1']. '</td>
					<td>'.$row['date1'].'</td>
				</tr>';
			
		}
          mysql_close($conn);
          ?>
		</tbody>
		<tfoot>
			<tr>
				
			</tr>
		</tfoot>
	</table>
</body>
</html>