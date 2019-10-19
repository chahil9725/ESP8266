<?php
$db_host = "www.db4free.net"; // Server Name
$db_user = "chahilpatel";// Username
$db_pass = "8866551417"; // Password
$db_name = "esp8266"; // Database Name

$conn = mysqli_connect($db_host, $db_user, $db_pass, $db_name);
if (!$conn) {
	die ('Failed to connect to MySQL: ' . mysqli_connect_error());	
}

$sql = 'SELECT * 
		FROM studentdata ORDER BY No desc';
		
$query = mysqli_query($conn, $sql);

if (!$query) {
	die ('SQL Error: ' . mysqli_error($conn));
}
?>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>

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
<center>	<h1>This Database Is Managed By Chahil9725</h1></center>
	<table class="data-table">
	</center>	<h3><caption class="title">Student Fingerprint Database</caption></h3></center>
		<thead>
			<tr>
                <th>NO</th>
				<th>ENROLLMENT</th>
				<th>NAME</th>
				<th>ID</th>
				<th>DATE</th>
				<th>TIME</th>
			</tr>
		</thead>
		<tbody>
		<?php
		$no 	= 1;
		$total 	= 0;
		while ($row = mysqli_fetch_array($query))
		{
		
			echo '<tr>
                    <td>'.$row['No'].'</td>
					<td>'.$row['enroll'].'</td>
					<td>'.$row['name'].'</td>
					<td>'.$row['id'].'</td>
					<td>'. date('F d, Y', strtotime($row['date1'])) . '</td>
					<td>'.$row['time1'].'</td>
				</tr>';
			
		}?>
		</tbody>
		<tfoot>
			
		</tfoot>
	</table>
</body>
</html>
